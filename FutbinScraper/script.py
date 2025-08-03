import requests
from bs4 import BeautifulSoup
from PIL import Image, ImageDraw, ImageFont
from io import BytesIO
import os
import time

HEADERS = {"User-Agent": "Mozilla/5.0"}
BASE_URL = 'https://www.futbin.com'
LISTING_URL = 'https://www.futbin.com/players?version=gold_rare&page={}'
SAVE_DIR = '/Users/jacksonwhite/CodingProjects/FifaPackOpener/cards'
FONT_PATH = "/Library/Fonts/Arial Bold.ttf"
FONT_SIZE = 24

os.makedirs(SAVE_DIR, exist_ok=True)


def fetch_image(url):
    response = requests.get(url)
    response.raise_for_status()
    return Image.open(BytesIO(response.content)).convert("RGBA")


def extract_player_info(soup):
    card_soup = soup.select_one('.playercard-25-name-stats-info-wrapper')
    name_tag = card_soup.select_one('.playercard-25-name') if card_soup else None
    name = name_tag.text.strip() if name_tag else "Unknown"

    card_div = soup.select_one('.playercard-25.playercard-l')
    card_color = None
    if card_div and 'style' in card_div.attrs:
        import re
        match = re.search(r'--cardColor:\s*(#[0-9a-fA-F]{6})', card_div['style'])
        if match:
            card_color = match.group(1)

    stats = {}
    stat_blocks = soup.select('.playercard-25-extended-stats .playercard-25-stats')
    for block in stat_blocks:
        label = block.select_one('.playercard-25-stat-value')
        value = block.select_one('.playercard-25-stat-number')
        if label and value:
            stats[label.text.strip()] = value.text.strip()

    def get_icon_url(alt_text):
        tag = soup.select_one(f'img[alt="{alt_text}"]')
        return tag['src'] if tag else None

    return {
        "name": name,
        "card_color": card_color,
        "stats": stats,
        "icons": {
            "Nation": get_icon_url("Nation"),
            "League": get_icon_url("League"),
            "Club": get_icon_url("Club"),
        }
    }


def generate_card_image(bg_url, face_url, info, width, height, player_width, player_height):
    bg = fetch_image(bg_url).resize((width, height))
    face = fetch_image(face_url).resize((player_width, player_height))
    face_x = (width - face.width) // 2 + 5
    face_y = 30
    bg.paste(face, (face_x, face_y), mask=face)

    draw = ImageDraw.Draw(bg)
    font = ImageFont.truetype(FONT_PATH, FONT_SIZE)

    text = info["name"]
    text_bbox = draw.textbbox((0, 0), text, font=font)
    text_width = text_bbox[2] - text_bbox[0]
    x = (width - text_width) // 2
    y = int(height * 0.65)
    draw.text((x, y), text, fill=info.get("color", info['card_color']), font=font)

    return bg


def get_all_player_links():
    print("🔍 Collecting player links...")
    player_links = []
    for page in range(1, 100):  # Arbitrary high number, we break when no players
        print(f"📄 Scraping page {page}...")
        url = LISTING_URL.format(page)
        response = requests.get(url, headers=HEADERS)
        soup = BeautifulSoup(response.text, "html.parser")
        table = soup.select_one('table.futbin-table.players-table')
        if not table:
            print("❌ No more players found.")
            break

        for row in table.select('tbody tr'):
            a_tag = row.select_one('a[href^="/25/player/"]')
            if a_tag:
                href = a_tag['href']
                full_url = BASE_URL + href
                player_links.append(full_url)

        time.sleep(1)  # Be kind to the server
    return player_links


def scrape_and_generate_card(player_url):
    print(f"🧪 Processing: {player_url}")
    try:
        response = requests.get(player_url, headers=HEADERS)
        response.raise_for_status()
        soup = BeautifulSoup(response.text, "html.parser")

        bg_tag = soup.select_one('.playercard-25-bg')
        face_tag = bg_tag.find_next_sibling('img') if bg_tag else None
        if not bg_tag or not face_tag:
            print("⚠️ Skipping player: images not found.")
            return

        bg_url = bg_tag['src']
        face_url = face_tag['src']
        card_width = int(bg_tag.get('width', 252))
        card_height = int(bg_tag.get('height', 300))
        player_width = int(face_tag.get('width', 252))
        player_height = int(face_tag.get('height', 300))

        info = extract_player_info(soup)
        filename = f"{info['name'].replace(' ', '_')}.png"
        save_path = os.path.join(SAVE_DIR, filename)

        image = generate_card_image(bg_url, face_url, info, card_width, card_height, player_width, player_height)
        image.save(save_path)
        print(f"✅ Saved: {filename}")
    except Exception as e:
        print(f"❌ Failed: {player_url}")
        print("   ", e)


def main():
    player_links = get_all_player_links()
    print(f"🔗 Found {len(player_links)} player links.")
    for url in player_links:
        scrape_and_generate_card(url)


if __name__ == "__main__":
    main()

