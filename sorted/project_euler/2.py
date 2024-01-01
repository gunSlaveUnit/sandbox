#!/usr/bin/env python

"""Even Fibonacci Numbers"""

s = 0
n = 4_000_000

a = 1
b = 2

s += b

while (a < n):
	a, b = b, a + b
	
	if not b & 1:
		s += b

print(s)
