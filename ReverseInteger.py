# !usr/bin/env/python3
# -*- coding: utf-8 -*-

# leetcode 7 easy

def reverse(x):
    if x > 0:
        return str(x)[::-1]
    else:
        x_reversed = '-' + str(abs(x))[::-1]
        return x_reversed


print(reverse(82173489123))
print(int("-"+str(-321)[:0:-1]))
