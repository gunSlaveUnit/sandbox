#! usr/bin/env python3
# -*- coding: utf8 -*-

def factorial_recursion(number):
    if number < 0:
        return
    if number == 0:
        return 1
    return number * factorial_recursion(number - 1)


def factorial_iteration_while(number):
    if number < 0:
        return
    result = 1
    while number > 1:
        result *= number
        number -= 1
    return result


def factorial_iteration_for(number):
    if number < 0:
        return
    result = 1
    for i in range(2, number+1):
        result *= i
    return result


if __name__ == '__main__':
    try:
        number_to_count_factorial = int(input('Please, enter a number to calculate factorial: '))

        print(f'Recursion Calculated Factorial of {number_to_count_factorial} is '
              f'{factorial_recursion(number_to_count_factorial)}')

        print(f'Iteration Calculated Factorial of {number_to_count_factorial} is '
              f'{factorial_iteration_while(number_to_count_factorial)}')
    except TypeError:
        print('Number is not an integer. Please, re-enter')
