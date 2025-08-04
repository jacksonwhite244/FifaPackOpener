import requests
from bs4 import BeautifulSoup
from PIL import Image, ImageDraw, ImageFont
from io import BytesIO
import os
import time

HEADERS = {
    "User-Agent": "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36",
    "Accept": "text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8",
    "Accept-Language": "en-US,en;q=0.5",
    "Accept-Encoding": "gzip, deflate, br",
    "Connection": "keep-alive",
    "Referer": "https://www.futbin.com/",
    "DNT": "1",  # Do Not Track
}

BASE_URL = 'https://www.futbin.com'
LISTING_URL = 'https://www.futbin.com/players?page={}&version=gold_rare'
SAVE_DIR = '/Users/jacksonwhite/CodingProjects/FifaPackOpener/cards'
FONT_PATH = "/Library/Fonts/Arial Bold.ttf"
RATING_FONT = ImageFont.truetype("/Library/Fonts/Arial Bold.ttf", 28)  # Bold and prominent
POSITION_FONT = ImageFont.truetype("/Library/Fonts/Arial.ttf", 15)     # Small and subtle
FONT_SIZE = 24

response = requests.get(LISTING_URL, headers=HEADERS)
print(f"Status code: {response.status_code}")
print(response.text[:500])

os.makedirs(SAVE_DIR, exist_ok=True)


def fetch_image(url):
    response = requests.get(url)
    response.raise_for_status()
    return Image.open(BytesIO(response.content)).convert("RGBA")

def sanitize_filename(name):
    # Lowercase, remove special characters, replace spaces with underscores
    return ''.join(c for c in name if c.isalnum() or c in (' ', '_')).replace(' ', '_').lower()


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

    # Add rating and position
    rating_pos_wrapper = soup.select_one('.playercard-25-rating-pos-wrapper')
    if rating_pos_wrapper:
        rating_tag = rating_pos_wrapper.select_one('.playercard-25-rating')
        position_tag = rating_pos_wrapper.select_one('.playercard-25-position')

        rating = rating_tag.text.strip() if rating_tag else "??"
        position = position_tag.text.strip() if position_tag else "??"

        stats["RAT"] = rating
        stats["POS"] = position


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
    # Center player face horizontally with a slight offset
    face_x = (width - face.width) // 2 + 5
    face_y = 60
    bg.paste(face, (face_x, face_y), mask=face)

    draw = ImageDraw.Draw(bg)
    font = ImageFont.truetype(FONT_PATH, FONT_SIZE)
    small_font = ImageFont.truetype(FONT_PATH, 18)  # smaller font for rating/position

    # Player name centered
    text = info["name"]
    text_bbox = draw.textbbox((0, 0), text, font=font)
    text_width = text_bbox[2] - text_bbox[0]
    x = (width - text_width) // 2
    y = int(height * 0.64)
    draw.text((x, y), text, fill=info.get("color", info['card_color']), font=font)

    # Rating (e.g., "91") - top-left aligned or centered if needed
    rating = info["stats"].get("RAT") or info["stats"].get("Rating") or "??"
    rating_bbox = draw.textbbox((0, 0), rating, font=RATING_FONT)
    rating_width = rating_bbox[2] - rating_bbox[0]
    rating_x = 57 - rating_width // 2  # Centered around x=35
    rating_y = 73
    draw.text((rating_x, rating_y), rating, fill=info.get("color", info['card_color']), font=RATING_FONT)

    # Position (e.g., "ST") - slightly below rating
    position = info["stats"].get("POS") or info["stats"].get("Position") or "??"
    position_bbox = draw.textbbox((0, 0), position, font=POSITION_FONT)
    position_width = position_bbox[2] - position_bbox[0]
    position_x = 56 - position_width // 2  # Same center x as rating
    position_y = rating_y + 26  # 26 pixels lower; tweak if needed
    draw.text((position_x, position_y), position, fill=info.get("color", info['card_color']), font=POSITION_FONT)

    # Paste nation flag icon top-right (adjust coords as needed)
    nation_url = info["icons"].get("Nation")
    if nation_url:
        flag = fetch_image(nation_url).resize((22, 15))
        bg.paste(flag, (width // 2 - 25 - 12, 288), mask=flag)

    # Paste club icon below nation flag
    club_url = info["icons"].get("Club")
    if club_url:
        club = fetch_image(club_url).resize((22, 22))
        bg.paste(club, (width // 2 + 25 - 11, 285), mask=club)

    league_url = info["icons"].get("League")
    if league_url:
        league = fetch_image(league_url).resize((22, 22))
        bg.paste(league, (width // 2 - 11, 285), mask=league)

    return bg

def get_all_player_links():
    print("Collecting player links...")
    player_links = []
    for page in range(1, 100):  # max pages
        print(f"Scraping page {page}...")
        url = LISTING_URL.format(page)
        response = requests.get(url, headers=HEADERS)
        soup = BeautifulSoup(response.text, "html.parser")

        tbody = soup.find('tbody', class_='with-border with-background')
        if not tbody:
            print("No tbody found on page, stopping.")
            break

        rows = tbody.find_all('tr', class_='player-row text-nowrap')
        if not rows:
            print("No player rows found on page, stopping.")
            break

        new_links = []
        seen_links = set()
        for row in rows:
            link_tag = row.find('td', class_='table-name').find('a', href=True)
            if link_tag:
                href = link_tag['href']
                full_url = BASE_URL + href
                if full_url not in seen_links:
                    seen_links.add(full_url)
                    new_links.append(full_url)

        if not new_links:
            print("No new players found on page, stopping.")
            break

        player_links.extend(new_links)
        time.sleep(3)  # be kind to server

    return player_links


def scrape_and_generate_card(player_url):
    print(f"Processing: {player_url}")
    try:
        response = requests.get(player_url, headers=HEADERS)
        response.raise_for_status()
        soup = BeautifulSoup(response.text, "html.parser")

        bg_tag = soup.select_one('.playercard-25-bg')
        face_tag = bg_tag.find_next_sibling('img') if bg_tag else None
        if not bg_tag or not face_tag:
            print("Skipping player: images not found.")
            return

        bg_url = bg_tag['src']
        face_url = face_tag['src']
        card_width = int(bg_tag.get('width', 252))
        card_height = int(bg_tag.get('height', 350))
        player_width = int(face_tag.get('width', 252))
        player_height = int(face_tag.get('height', 300))

        info = extract_player_info(soup)
        filename = f"{info['name'].replace(' ', '_')}.png"
        save_path = os.path.join(SAVE_DIR, filename)

        image = generate_card_image(bg_url, face_url, info, card_width, card_height, player_width, player_height)

        # Construct filename
        name = info["name"].lower().replace(" ", "_")
        card_type = info.get("card_type", "gold-rare").lower()  # use "gold" if you know it's gold
        rating = info["stats"].get("RAT") or info["stats"].get("Rating") or "??"
        position = info["stats"].get("POS") or info["stats"].get("Position") or "??"
        filename = f"{name}_{card_type}_{rating}_{position}.png"

        save_path = os.path.join(SAVE_DIR, filename)
        image.save(save_path)
        print(f"Saved: {filename}")
    except Exception as e:
        print(f"Failed: {player_url}")
        print("   ", e)


def main():
    player_links = get_all_player_links()
    print(f"Found {len(player_links)} player links.")
    for url in player_links:
        scrape_and_generate_card(url)


if __name__ == "__main__":
    main()