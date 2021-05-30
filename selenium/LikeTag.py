# !usr/bin/env python3
# -*- coding: utf-8 -*-

from time import sleep
from random import randrange

from selenium import webdriver
from selenium.webdriver.common.keys import Keys

try:
    from Settings import USERNAME, PASSWORD, DRIVER_PATH, LINK
except ImportError:
    from SettingsDefault import USERNAME, PASSWORD, DRIVER_PATH, LINK
    print('Fill vars in SettingsDefault.py and rename it to Settings.py')


def hashtag_search(hashtag):
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

        sleep(5)

        browser.get(LINK + f'/explore/tags/{hashtag}/')

        sleep(5)

        for _ in range(1, 5):
            browser.execute_script('window.scrollTo(0, document.body.scrollHeight);')
            sleep(randrange(5, 7))

        links = browser.find_elements_by_tag_name('a')
        urls = [link.get_attribute('href') for link in links if '/p/' in link.get_attribute('href')]
        for url in urls[0:1]:
            browser.get(url)
            sleep(3)
            like_button = browser.find_element_by_xpath(
                '/html/body/div[5]/div[2]/div/article/div[3]/section[1]/span[1]/button').click()
            sleep(randrange(80, 100))
    except Exception as ex:
        print(ex)
    finally:
        browser.close()
        browser.quit()


def main():
    hashtag_search('dogs')


if __name__ == '__main__':
    main()
