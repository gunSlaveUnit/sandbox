#! usr/bin/env python3
# -*- coding: utf8 -*-

from PIL import Image


def main():
    image_name = input('Enter an image name: ')
    image = Image.open(image_name)
    black_white_image = image.convert("L")
    black_white_image.show()


if __name__ == '__main__':
    main()
