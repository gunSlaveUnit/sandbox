# !usr/bin/env python3
# -*- coding: utf-8 -*-

def karatsuba(x, y):
    # Base case
    if x < 10 or y < 10:
        return x * y
    # Calculate the number of digits of the numbers
    sx, sy = str(x), str(y)
    m2 = max(len(sx), len(sy)) // 2
    # Split the digit sequences about the middle
    ix, iy = len(sx) - m2, len(sy) - m2
    a, b = int(sx[:ix]), int(sx[ix:])
    c, d = int(sy[:iy]), int(sy[iy:])
    # 3 products of numbers with half the size
    A = karatsuba(a, c)
    C = karatsuba(b, d)
    D = karatsuba(a + b, c + d)
    return A * 10 ** (2 * m2) + (D - A - C) * 10 ** m2 + C


if __name__ == '__main__':
    print(karatsuba(5324542837123, 912312312313))
