#! usr/bin/env python3
# -*- coding: utf8 -*-

SUFFIXES = {1000: ['KB', 'MB', 'GB', 'TB', 'PB', 'EB', 'ZB', 'YB'],
            1024: ['KiB', 'MiB', 'GiB', 'TiB', 'PiB', 'EiB', 'ZiB', 'YiB']}


def approximate_size(size, a_kilobyte_is_1024_bytes=True):
    if size < 0:
        raise ValueError("The number must be > 0.")

    multiple = 1024 if a_kilobyte_is_1024_bytes else 1000
    for suffix in SUFFIXES[multiple]:
        size /= multiple
        if size < multiple:
            return '{0:.1f} {1}.'.format(size, suffix)

    raise ValueError("The number is too big.")


if __name__ == '__main__':
    print(approximate_size(17846237856456, False))
    print(approximate_size(17846237856456))
