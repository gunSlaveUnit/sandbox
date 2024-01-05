#!/usr/bin/env python

"""Sum Square Difference"""

if __name__ == "__main__":
	n = [i for i in range(1, 101)]

	qs = int(pow(sum(n), 2))
	sq = sum([int(pow(i, 2)) for i in n])

	print(qs - sq)
