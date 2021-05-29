# !usr/bin/env python3
# -*- coding: utf-8 -*-

from time import sleep
from random import randrange

from selenium import webdriver
from selenium.webdriver.common.keys import Keys

try:
    from settings import USERNAME, PASSWORD, DRIVER_PATH, LINK
except ImportError:
    print('Fill vars in settings_default.py and rename it to settings.py')


def login():
    browser = webdriver.Chrome(DRIVER_PATH)
    browser.get(LINK)
    try:
        sleep(randrange(1, 3))

        input_username_field = browser.find_element_by_name('username')
        input_username_field.clear()
        input_username_field.send_keys(USERNAME)

        sleep(randrange(1, 2))

        input_password_field = browser.find_element_by_name('password')
        input_password_field.clear()
        input_password_field.send_keys(PASSWORD)
        input_password_field.send_keys(Keys.ENTER)

        sleep(20)
    except Exception as ex:
        print(ex)
    finally:
        browser.close()
        browser.quit()


def main():
    login()


if __name__ == '__main__':
    main()
