import random
import sys

DEFAULT_ENCRYPT_FILENAME = "decoded.txt"
DEFAULT_DECRYPT_FILENAME = "encoded.txt"
DEFAULT_KEY_FILENAME = "key.txt"


def read_file_chunk(file_object, chunk_size=1024):
    while True:
        data = file_object.read(chunk_size)
        if not data:
            break
        yield data


def generate_key(size: int):
    key = [i for i in range(size)]
    random.shuffle(key)
    return key


def encrypt(filename: str, m: int, n: int) -> None:
    buffer_size = m * n

    with open(filename, 'r') as input_file:
        with open(DEFAULT_DECRYPT_FILENAME, 'w') as output_file:
            key = generate_key(m)

            for chunk in read_file_chunk(input_file, buffer_size):
                if len(chunk) != buffer_size:
                    additional_spaces_amount = buffer_size - len(chunk)
                    chunk += " " * additional_spaces_amount

                encoded_chunk_symbols = ['' for _ in range(buffer_size)]

                for u in range(n):
                    for i, v in enumerate(key):
                        encoded_chunk_symbols[i + u * m] = chunk[v + u * m]

                encoded_chunk = ''.join(encoded_chunk_symbols)
                output_file.write(encoded_chunk)

            with open(DEFAULT_KEY_FILENAME, 'w') as key_file:
                key_file.write(' '.join([str(v) for v in key]))


def decrypt(filename: str, m: int, n: int, keyfile: str) -> None:
    buffer_size = m * n

    with open(filename, 'r') as input_file:
        with open(DEFAULT_ENCRYPT_FILENAME, 'w') as output_file:
            with open(keyfile, 'r') as key_file:
                key = key_file.readline().split()
                key = [int(v) for v in key]

            for chunk in read_file_chunk(input_file, buffer_size):
                decoded_chunk_symbols = ['' for _ in range(buffer_size)]
                for u in range(n):
                    for i, v in enumerate(key):
                        decoded_chunk_symbols[v + u * m] = chunk[i + u * m]

                encoded_chunk = ''.join(decoded_chunk_symbols)
                output_file.write(encoded_chunk)


def main() -> None:
    mode = sys.argv[1]
    filename = sys.argv[2]
    m = int(sys.argv[3])
    n = int(sys.argv[4])
    if mode == '-e':
        encrypt(filename, m, n)
    elif mode == '-d':
        decrypt(filename, m, n, sys.argv[5])
    else:
        print("ERROR: Unrecognized working mode. Try -h or --help.")


if __name__ == '__main__':
    main()