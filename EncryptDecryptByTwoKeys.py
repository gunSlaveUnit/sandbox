#! usr/bin/env python3
# -*- coding: utf8 -*-

from secrets import token_bytes
from typing import Tuple


def random_key(length: int) -> int:
    tb: bytes = token_bytes(length)

    return int.from_bytes(tb, 'big')


def encrypt(original: str) -> Tuple:
    original_bytes: bytes = original.encode()
    dummy: int = random_key(len(original_bytes))
    original_key: int = int.from_bytes(original_bytes, 'big')
    encrypted: int = original_key ^ dummy
    return dummy, encrypted


def decrypt(key_1: int, key_2: int) -> str:
    decrypted: int = key_1 ^ key_2
    temp: bytes = decrypted.to_bytes(
        (decrypted.bit_length() + 7) // 8, 'big')
    return temp.decode()


if __name__ == '__main__':
    key1, key2 = encrypt('Some string')
    result: str = decrypt(key1, key2)
    print(result)
