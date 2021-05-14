# !usr/bin/env/python3
# -*- coding: utf-8 -*-

class ListNode:
    def __init__(self, x):
        self.value = x
        self.next = None


def reverse_list(head):
    def reverse_list_recursively(previous, current):
        global head_local
        head_local = current
        if current.next is not None:
            head_local = reverse_list_recursively(current, current.next)
        current.next = previous
        return head

    if head is None: return None
    return reverse_list_recursively(None, head)


global head_local
if __name__ == '__main__':
    pass
