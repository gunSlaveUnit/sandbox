#!/usr/bin/env python

"""Largest Prime Factor"""

if __name__ == "__main__":
	n = 600851475143
	factor = 3
	last = factor

	while n > 1:
		if n % factor == 0:
			last = factor

			while n % factor == 0:
				n //= factor

		factor += 2

	print(last)
