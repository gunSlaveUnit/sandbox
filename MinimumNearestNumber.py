#! usr/bin/env python3
# -*- coding: utf8 -*-

# It doesn't work yet

number = [3, 2, 1, 0]


def reverse(start: int, end: int) -> None:
    amount_swaps: int = (end-start+1)//2

    for i in range(amount_swaps-1):
        number[i], number[i+1] = number[i+1], number[i]


def get_index_swap_number() -> int:
    for i in range(len(number)-1, 0, -1):
        if number[i-1] > number[i]:
            return i-1
    return -1


def get_index_swap_with_number(swap_value) -> int:
    for i in range(len(number)-1, 0, -1):
        if number[i] < swap_value:
            return i
    return -1


def get_nearest_number():
    swap_number = get_index_swap_number()
    if swap_number == -1: return None

    swap_with_number = get_index_swap_with_number(number[swap_number])
    swap_number, swap_with_number = swap_with_number, swap_number

    reverse(swap_number+1, len(number)-1)

    return number


if __name__ == '__main__':
    print(get_nearest_number())
