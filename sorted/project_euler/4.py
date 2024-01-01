#!/usr/bin/env python

"""Largest Palindrome Product"""

if __name__ == "__main__":
	last = 0

	for i in range(999, 900, -1):
		for j in range(i, 1000):
			p = i * j
			
			f = p // 100_000 % 10 == p % 10
			s = p // 10_000 % 10 == p // 10 % 10
			t = p // 1_000 % 10 == p // 100 % 10

			if f and s and t and p > last:
				last = p

	print(last)
