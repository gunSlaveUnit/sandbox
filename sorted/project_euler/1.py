"""Multiples of 3 or 5"""

s = 0
n = 1000

for i in range(3, n, 3):
	s += i

for i in range(5, n, 5):
	s += i

for i in range(15, n, 15):
	s -= i

print(s)
