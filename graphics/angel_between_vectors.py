"""A simple geometric problem: find the angle between two vectors."""

import math

a = (3, -2)
b = (1, 7)

dot_product = a[0] * b[0] + a[1] * b[1]

magnitude_a = (a[0] ** 2 + a[1] ** 2) ** 0.5
magnitude_b = (b[0] ** 2 + b[1] ** 2) ** 0.5

angle = math.acos(dot_product / (magnitude_a * magnitude_b))

print(angle * 180 / math.pi)
