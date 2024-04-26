# !usr/bin/env python3
# -*- coding: utf-8 -*-

from PIL import Image

operating_mode = input("Enter an act (hide or extract): ")

name_image = input("Enter an image name: ")
original_image = Image.open(name_image)
original_image = original_image.convert('RGB')
width, height = original_image.size

if operating_mode == 'hide':
    message_to_hide = input("Enter a message: ")
    message_to_hide += '~'

    bits = str()
    for symbol in message_to_hide:
        symbol = str(bin(ord(symbol)))
        symbol = symbol.replace('0b', '')
        if len(symbol) == 6:
            symbol = "00" + symbol
        elif len(symbol) == 7:
            symbol = "0" + symbol
        bits += symbol
    bits = list(bits)

    modified_image = Image.new('RGB', (width, height))

    idx = 0
    for i in range(width):
        for j in range(height):
            r, g, b = original_image.getpixel((i, j))
            if idx < len(bits):
                if bits[idx] == '0':
                    r &= 254
                else:
                    r |= 1
            idx += 1

            modified_image.putpixel((i, j), (r, g, b))

    modified_image.save(name_image.split('.')[0] + 'WithMessage.' + name_image.split('.')[-1])
elif operating_mode == 'extract':
    bits = ''
    for i in range(width):
        for j in range(height):
            r, g, b = original_image.getpixel((i, j))
            last_bit = str(bin(r))[-1]
            bits += last_bit

    message = ''
    for i in range(0, len(bits)-8, 8):
        character = '0b' + bits[i:i+8]
        message += chr(int(character, 2))
        if chr(int(character, 2)) == '~':
            break

    message = message.replace('~', '')

    print('Your message is: \n' + message)
else:
    print("Unknown operating mode.")
