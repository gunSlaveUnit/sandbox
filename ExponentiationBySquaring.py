#! usr/bin/env python3
# -*- coding: utf8 -*-

def power(a, n):
    if n == 0:
        return 1
    elif n == 1:
        return a
    elif n % 2 != 0:
        return a * power(a, n-1)
    elif n % 2 == 0:
        return power(a*a, n/2)


if __name__ == '__main__':
    print(power(2, 1))
