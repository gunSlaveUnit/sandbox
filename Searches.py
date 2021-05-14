# !usr/bin/env python3
# -*- coding: utf-8 -*-

def linear_search(sequence, item_to_search):
    for index in range(len(sequence)):
        if sequence[index] == item_to_search:
            return index
    return -1


def binary_search(sequence, item_to_search):
    if len(sequence) == 0 or (len(sequence)==1 and sequence[0]!=item_to_search):
        return -1
    left = 0
    right = len(sequence) - 1
    while left <= right:
        middle = left+(right-left)//2
        if sequence[middle] == item_to_search:
            return middle
        if item_to_search < sequence[middle]:
            right = middle - 1
        else:
            left = middle + 1
    return -1


def binary_search_two(sequence, target):
    if not sequence:
        return -1

    l = 0
    r = len(sequence)

    while l+1 < r:
        m = l + (r-l)//2
        if sequence[m] > target:
            r = m
        else:
            l = m

    return l if sequence[l] == target else -1


if __name__ == '__main__':
    print(binary_search([10, 20, 30, 40, 50], 23))
    print(binary_search_two([10, 20, 30, 40, 50], 23))
