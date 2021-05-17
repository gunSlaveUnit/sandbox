import os

import requests
from bs4 import BeautifulSoup

image_number = 0
page_number = 1
LINK = f'https://wallpaperscraft.com'

resolution = input('Enter resolution in format widthxheight: ')

response_text = requests.get(f'{LINK}/all/page{page_number}').text
soup = BeautifulSoup(response_text, 'lxml')
images = soup.find('ul', class_='wallpapers__list')
images_links = images.find_all('a')

for link in images_links:
    images_link = link.attrs['href']
    downloading_page = requests.get(f'{LINK}/{images_link}').text
    download = BeautifulSoup(downloading_page, 'lxml')
    link_downloading_page = download.find('a', class_='resolutions__link', text=f'{resolution}')

    link_to_page_with_downloading_button = link_downloading_page.attrs['href']
    page_with_download_button = requests.get(f'{LINK}{link_to_page_with_downloading_button}').text
    searching_download_button = BeautifulSoup(page_with_download_button, 'lxml')
    download_button = searching_download_button.find('a', class_='gui-button gui-button_full-height')

    link_to_image = download_button.attrs['href']
    image = requests.get(link_to_image).content

    with open(os.path.join(os.path.abspath('images'), f'{image_number}.jpg'), 'wb') as file:
        file.write(image)

    image_number += 1
    print(f'Image {image_number} was downloaded successful')
