#! usr/bin/env python3
# -*- coding: utf8 -*-

import requests
import fake_useragent
from bs4 import BeautifulSoup

USER_AGENT = fake_useragent.UserAgent().random
LINK_PARSING = "https://2ip.ru/"
HEADERS = {'user=-agent': USER_AGENT}


def main():
    response = requests.get(url=LINK_PARSING, headers=HEADERS).text
    soup = BeautifulSoup(response, 'lxml')

    ip_block = soup.find('div', id='d_clip_button')
    ip = ip_block.find('span').text

    data_table_block = soup.find('div', 'data_table')

    print(f'IP: {ip}')
    print(data_table_block)
    print(USER_AGENT)


if __name__ == '__main__':
    main()
