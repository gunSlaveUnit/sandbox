# !usr/bin/env/python3
# -*- coding: utf-8 -*-

def calculate_water(heights):
    if not heights:
         return 0

    max_element_index = 0

    for index in range(len(heights)):
        if heights[index] > heights[max_element_index]:
            max_element_index = index

    result = 0

    max_left = 0
    for index in range(max_element_index):
        if heights[index] < max_left:
            result += max_left - heights[index]

        max_left = max(max_left, heights[index])

    max_right = 0
    for index in range(len(heights)-1, max_element_index, -1):
        if heights[index] < max_right:
            result += max_right - heights[index]

        max_right = max(max_right, heights[index])

    return result

if __name__ == '__main__':
    print(calculate_water([3, 5, 3, 4, 6, 5, 3]))
