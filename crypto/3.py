import random

BACKPACK_SIZE = 256
INCREASING_SEQUENCE_INIT_NUMBER = 10 ** 100
BLOCK_LENGTH_BYTES = BACKPACK_SIZE >> 3


def gcd(a, b):
    if b == 0:
        return a
    else:
        return gcd(b, a % b)


def egcd(a, b):
    if a == 0:
        return b, 0, 1
    else:
        g, y, x = egcd(b % a, a)
        return g, x - (b // a) * y, y


def modinv(a, m):
    g, x, y = egcd(a, m)
    if g != 1:
        raise Exception('No modular inverse')
    else:
        return x % m


def gen_super_increasing_sequence(n):
    s = [INCREASING_SEQUENCE_INIT_NUMBER]
    for i in range(n - 1):
        s.append(s[i] + random.randint(s[i], 2 * s[i]))
    return s


def gen_public_key(s):
    m = random.randint(sum(s), 2 * sum(s))
    while True:
        n = random.randint(2, m - 1)
        if gcd(n, m) == 1:
            break
    w = [n * si % m for si in s]
    return w, n, m


def encrypt(plain_text, public_key):
    cipher_text = []

    for i in range(0, len(plain_text), BLOCK_LENGTH_BYTES):
        block = plain_text[i:i + BLOCK_LENGTH_BYTES]
        bits = "".join([format(ord(c), '08b') for c in block])
        c = sum([int(bits[j]) * public_key[j] for j in range(len(bits))])
        cipher_text.append(c)

    return cipher_text


def decrypt(cipher_text, private_key, n, m):
    plain_text = ''
    for c in cipher_text:
        ci = c * modinv(n, m) % m
        bi = ''
        for si in reversed(private_key):
            if si <= ci:
                bi += '1'
                ci -= si
            else:
                bi += '0'

        bi = bi[::-1]

        for i in range(0, BACKPACK_SIZE, 8):
            plain_text += chr(int(bi[i: i + 8], 2))

    return plain_text


def main():
    private_key = gen_super_increasing_sequence(BACKPACK_SIZE)
    public_key, n, m = gen_public_key(private_key)

    message = 'The quick brown fox jumps over the lazy dog. Jackdaws love my big sphinx of quartz. The five boxing ' \
              'wizards jump quickly. How vexingly quick daft zebras jump. Bright vixens jump; dozy fowl quack. The ' \
              'jay, pig, fox, zebra and my wolves quack! Blowzy red vixens fight for a quick jump. Joaquin Phoenix ' \
              'was gazed by MTV for luck. Crazy Fredrick bought many very exquisite opal jewels. Quick zephyrs blow, ' \
              'vexing daft Jim. Sphinx of black quartz, judge my vow.'

    print("Plain: ", message)
    cipher_text = encrypt(message, public_key)

    print("Cipher: ", cipher_text)
    print(cipher_text)

    decrypted = decrypt(cipher_text, private_key, n, m)
    print("Decrypted: ", decrypted)


if __name__ == '__main__':
    main()
