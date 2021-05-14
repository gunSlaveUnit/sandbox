#! usr/bin/env python3
# -*- coding: utf8 -*-

"""
Most of the functions in this file work by the number of the required number, not by its index

There is an option using matrices yet
"""


def fibonacci_my(index_number_generate):
    a, b = 0, 1
    for _ in range(2, index_number_generate):
        a, b = b, a+b
    return a


def fibonacci_list_my(index_number_generate):
    fibonacci_numbers = [0, 1]
    for i in range(0, index_number_generate-2):
        new_value = fibonacci_numbers[i]+fibonacci_numbers[i+1]
        fibonacci_numbers.append(new_value)
    return fibonacci_numbers


def fibonacci_recursive_my(index_number_generate):
    if index_number_generate == 1:
        return 0
    if index_number_generate == 2:
        return 1
    return fibonacci_recursive_my(index_number_generate-2) + fibonacci_recursive_my(index_number_generate-1)


def fibonacci_closed_formula(n):
    from math import sqrt
    phi = (sqrt(5)+1)/2

    return int(phi**(n-1)/sqrt(5)+0.5)


def fibonacci_generator():
    a, b = 0, 1
    while True:
        yield a
        a, b = b, a+b


class MatrixFibonacci:
    Q = [[1, 1],
         [1, 0]]

    def __init__(self):
        self.__memo = {}

    def __multiply_matrices(self, M1, M2):
        a11 = M1[0][0]*M2[0][0] + M1[0][1]*M2[1][0]
        a12 = M1[0][0]*M2[0][1] + M1[0][1]*M2[1][1]
        a21 = M1[1][0]*M2[0][0] + M1[1][1]*M2[1][0]
        a22 = M1[1][0]*M2[0][1] + M1[1][1]*M2[1][1]
        r = [[a11, a12], [a21, a22]]
        return r

    def __get_matrix_power(self, M, p):
        if p == 1:
            return M
        if p in self.__memo:
            return self.__memo[p]
        K = self.__get_matrix_power(M, int(p/2))
        R = self.__multiply_matrices(K, K)
        self.__memo[p] = R
        return R

    def get_number(self, n):
        if n == 0:
            return 0
        if n == 1:
            return 1
        powers = [int(pow(2, b))
                  for (b, d) in enumerate(reversed(bin(n-1)[2:])) if d == '1']

        matrices = [self.__get_matrix_power(MatrixFibonacci.Q, p)
                    for p in powers]
        while len(matrices) > 1:
            M1 = matrices.pop()
            M2 = matrices.pop()
            R = self.__multiply_matrices(M1, M2)
            matrices.append(R)
        return matrices[0][0][0]


if __name__ == '__main__':
    print(fibonacci_my(50))
    # print(fibonacci_list_my(10))
    # print(fibonacci_recursive_my(30))
    # print(fibonacci_closed_formula(10**3))

    def fib(n): return fib(n-2) + fib(n-1) if n > 2 else 1  # it works by index
    # print(fib(30))

    generator = fibonacci_generator()
    for i in range(10):
        # print(next(generator), end=' ')
        pass

    mfib = MatrixFibonacci()
    for n in range(0, 128):
        num = mfib.get_number(n)
        print(num)
