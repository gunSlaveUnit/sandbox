#! usr/bin/env python3
# -*- coding: utf8 -*-

import requests

LINK = "https://stackoverflow.com/"

response = requests.get(LINK)
response_text = requests.get(LINK).text
response_content = requests.get(LINK).content
response_status = requests.get(LINK).status_code

if __name__ == '__main__':
    print(response_status)
