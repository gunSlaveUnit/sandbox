"""
Given an array of integers nums and an integer target, 
return indices of the two numbers such that they add up to target.
"""


def two_sum(nums: list[int], target: int) -> list[int, int]:
    m = {}

    for i, num in enumerate(nums):
        if (n := target - num) in m:
            return [i, m[n]]
        else:
            m[num] = i

    return []


print(two_sum([2, 3, 5, 10], 15))
print(two_sum([2, 7, 11, 15], 9))
