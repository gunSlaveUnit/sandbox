#! usr/bin/env python3
# -*- coding: utf8 -*-

import requests
import fake_useragent
from bs4 import BeautifulSoup

LINK = 'https://www.cyberforum.ru/posting.php?do=login'
USER = fake_useragent.UserAgent().random
HEADERS = {
    'user-agent': USER
}
DATA = {
    'vb_login_username': '',
    'vb_login_password': ''
}


def main():
    email = input('Please, enter your email: ')
    password = input('Please, enter your password: ')

    HEADERS['vb_login_username'] = email
    HEADERS['vb_login_password'] = password

    session = requests.Session()
    response = session.post(url=LINK, data=DATA, headers=HEADERS)
    print(response)

    cookies = [
        {"domain": key.domain,
         "name": key.name,
         "path": key.path,
         "value": key.value} for key in session.cookies
    ]

    session = requests.Session()
    for cookie in cookies:
        session.cookies.set(**cookie)


if __name__ == '__main__':
    main()
