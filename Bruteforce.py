import time

chars = r'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789`~!@#$%^&*()_-+=\|{[]};:'",./?"

password = 'XT$a23'

sequence = []

m = len(chars)
n = 10

for i in range(n):
    sequence.append('')


def rec(idx):
    if idx == n:
        if ''.join(sequence) == password:
            print(f'[Got it]: | {"".join(sequence)} |]')
        return
    for i in range(m):
        sequence[idx] = chars[i]
        rec(idx+1)


def main():
    global n
    for i in range(n+1):
        n = i
        rec(0)


if __name__ == '__main__':
    start = time.monotonic()
    main()
    print(rf'[ DONE\: {time.monotonic() - start} ]')
