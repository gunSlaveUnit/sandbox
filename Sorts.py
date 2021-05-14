# !usr/bin/env python3
# -*- coding: utf-8 -*-

import random


def create_array(length=10, bound=50):
    new_array = [random.randint(0, bound) for _ in range(length)]
    return new_array


def bubble_sort(array):
    swapped = True
    while swapped:
        swapped = False
        for i in range(1, len(array)):
            if array[i-1] > array[i]:
                array[i-1], array[i] = array[i], array[i-1]
                swapped = True
    return array


def selection_sort(array):
    sort_length = 0
    while sort_length < len(array):
        min_index = None
        for i, value in enumerate(array[sort_length:]):
            if min_index is None or value < array[min_index]:
                min_index = i + sort_length
        array[sort_length], array[min_index] = array[min_index], array[sort_length]
        sort_length += 1
    return array


def insertion_sort(array):
    for sort_length in range(1, len(array)):
        current_item = array[sort_length]
        insert_index = sort_length
        while insert_index > 0 and current_item < array[insert_index-1]:
            array[insert_index] = array[insert_index-1]
            insert_index -= 1
        array[insert_index]=current_item
    return array


def merge_sort(array):
    def merge(a, b):
        sorted_array = []
        a_index, b_index = 0, 0
        while a_index < len(a) and b_index < len(b):
            if a[a_index] < b[b_index]:
                sorted_array.append(a[a_index])
                a_index += 1
            else:
                sorted_array.append(b[b_index])
                b_index += 1
        if a_index == len(a):
            sorted_array.extend(b[b_index:])
        else:
            sorted_array.extend(a[a_index:])
        return sorted_array

    if len(array) <= 1:
        return array
    left, right = merge_sort(array[:len(array)//2]), merge_sort(array[len(array)//2:])
    return merge(left, right)


def quick_sort(array):
    if len(array) <= 1:
        return array
    smaller, equal, larger = [], [], []
    pivot = array[random.randint(0, len(array)-1)]
    for x in array:
        if x < pivot:
            smaller.append(x)
        elif x == pivot:
            equal.append(x)
        else:
            larger.append(x)
    return quick_sort(smaller)+equal+quick_sort(larger)


def quick_sort_inplace(a, low=0, high=None):
    def partition(a, low, high):
        i = low-1
        pivot = a[high]
        for j in range(low, high):
            if a[j] <= pivot:
                i += 1
                a[i], a[j] = a[j], a[i]
        a[i+1], a[high] = a[high], a[i+1]
        return i+1

    if high is None:
        high = len(a)-1
    if low < high:
        partition_index = partition(a, low, high)
        quick_sort_inplace(a, low, partition_index-1)
        quick_sort_inplace(a, partition_index+1, high)
    return a


def bogo_sort(a):
    def is_sorted(a):
        for i in range(1, len(a)):
            if a[i]<a[i-1]:
                return False
        return True
    while not is_sorted(a):
        random.shuffle(a)
    return a


if __name__ == '__main__':
    pass
