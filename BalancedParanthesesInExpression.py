#! usr/bin/env python3
# -*- coding: utf8 -*-

class Stack:
    def __init__(self):
        self.__content = []

    def push(self, obj):
        self.__content.insert(0, obj)

    def pop(self):
        self.__content.pop(0)

    def is_empty(self):
        if self.__content:
            return False
        return True


BRACES = [['{', '}'], ['[', ']'], ['(', ')']]


def is_balanced(expression):
    def is_open_brace(brace):
        for couple_braces in BRACES:
            if couple_braces[0] == brace:
                return True
        return False

    def is_couple_braces(open_brace, close_brace):
        for couple_braces in BRACES:
            if couple_braces[0] == open_brace:
                return couple_braces[1] == close_brace
        return False

    stack = Stack()
    for character in expression:
        if is_open_brace(character):
            stack.push(character)
        elif stack.is_empty() or is_couple_braces(stack.pop(), character):
            return False

    return stack.is_empty()


if __name__ == '__main__':
    expression_to_check_balance = '({{[(][)}){}}]'
    print(is_balanced(expression_to_check_balance))
