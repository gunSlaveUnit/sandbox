#! usr/bin/env python3
# -*- coding: utf8 -*-

from PIL import Image


def get_amount_pixels_to_cut():
    """
    Получает от пользователя количество пикселей для образки изображения с четырёх сторон.
    :return: кортеж из значений для обрезки: x, y, width, height
    """
    x = int(input('Введите координату X верхнего левого угла обрезки: '))
    y = int(input('Введите координату Y верхнего левого угла обрезки: '))
    width = int(input('Введите ширину обрезки: '))
    height = int(input('Введите высоту обрезки: '))
    return x, y, width, height


def get_image_name():
    """
    Получает от пользователя имя файла.
    :return: строка имени файла.
    """
    path = input('Введите путь к файлу: ')
    return path


if __name__ == '__main__':
    cutting_coordinates_and_pixels_amount = (x, y, width, height) = get_amount_pixels_to_cut()
    image_name = get_image_name()
    try:
        image = Image.open(image_name)
        cut_image = image.crop(cutting_coordinates_and_pixels_amount)
        cut_image.show()
    except Exception as ex:
        print(ex)
