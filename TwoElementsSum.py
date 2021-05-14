# !usr/bin/env/python3
# -*- coding: utf-8 -*-

# leetcode 1 easy

def is_sum_two_elements(nums, target):
    el = {}
    for i, cur_num in enumerate(nums):
        add_num = target - cur_num
        if add_num in el:
            return [el[add_num], i]
        el[cur_num] = i


if __name__ == '__main__':
    print(is_sum_two_elements([-1, -2, -3, -4, -5], -8))
