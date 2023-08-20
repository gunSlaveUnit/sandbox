#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "serpent-reference.h"

typedef struct {
    KEY content;
    keySchedule sub_keys;
} Key;

const char *DEFAULT_ENCRYPTED_FILENAME = "encrypted.bin";
const char *DEFAULT_DECRYPTED_FILENAME = "decrypted.txt";
const char *DEFAULT_KEY_FILENAME = "key.bin";
const int KEY_LENGTH_BITS = 256;
const int BLOCK_LENGTH_BITS = 128;

void form_key(Key *key, int length) {
    WORD w[132], k[132];
    for (int i = 0; i < length / 32; ++i)
        w[i] = key->content[i];
    for (int i = 8; i < 16; ++i)
        w[i] = ROL(w[i - 8] ^ w[i - 5] ^ w[i - 3] ^ w[i - 1] ^ phi ^ (i - 8), 11);
    for (int i = 0; i < 8; ++i)
        w[i] = w[i + 8];
    for (int i = 8; i < 132; ++i)
        w[i] = ROL(w[i - 8] ^ w[i - 5] ^ w[i - 3] ^ w[i - 1] ^ phi ^ i, 11);

    for (int i = 0; i < 33; ++i) {
        int s_index = i % 8;
        k[0 + 4 * i] = k[1 + 4 * i] = k[2 + 4 * i] = k[3 + 4 * i] = 0;
        for (int j = 0; j < 32; ++j) {
            BYTE input = makeByte(getBitFromWord(w[0 + 4 * i], j),
                                    getBitFromWord(w[1 + 4 * i], j),
                                    getBitFromWord(w[2 + 4 * i], j),
                                    getBitFromWord(w[3 + 4 * i], j));
            BYTE output = S(s_index, input);
            for (int l = 0; l < 4; l++)
                k[4 * i + l] |= ((WORD) getBitFromByte(output, l)) << j;
        }
    }

    for (int i = 0; i < 33; ++i)
        for (int j = 0; j < 4; ++j)
            key->sub_keys[i][j] = k[4 * i + j];
}

void encrypt_block(BLOCK input, int input_length, BLOCK output, keySchedule sub_keys) {
    for (int b = 0; b < input_length; b += BLOCK_LENGTH_BITS, input += 16, output += 16) {
        BLOCK buffer;

        IP(input, buffer);

        for (int i = 0; i < r; ++i)
            R(i, buffer, sub_keys, buffer);

        FP(buffer, output);
    }
}

void decrypt_block(BLOCK input, int input_length, BLOCK output, keySchedule sub_keys) {
    for (int b = 0; b < input_length; b += BLOCK_LENGTH_BITS, input += 16, output += 16) {
        BLOCK buffer;

        FPInverse(input, buffer);

        for (int i = r - 1; i >= 0; --i)
            RInverse(i, buffer, sub_keys, buffer);

        IPInverse(buffer, output);
    }
}

void encrypt(const char *source_filename) {
    FILE *source_file = fopen(source_filename, "rb");

    fseek(source_file, SEEK_END, 0);
    long file_length_bytes = ftell(source_file);
    unsigned char trash_bytes_in_last_block_amount = file_length_bytes % 16;

    FILE *output_file = fopen(DEFAULT_ENCRYPTED_FILENAME, "wb");
    fwrite(&trash_bytes_in_last_block_amount, sizeof(char), 1, output_file);

    fseek(source_file, SEEK_SET, 0);

    Key key;
    for (int i = 0; i < 8; ++i)
        key.content[i] = rand();

    form_key(&key, KEY_LENGTH_BITS);
    FILE *key_file = fopen(DEFAULT_KEY_FILENAME, "wb");
    fwrite(key.content, sizeof(WORD), 8, key_file);
    fclose(key_file);

    BLOCK block;
    while (fread(block, 4, 4, source_file)) {
        encrypt_block(block, 128, block, key.sub_keys);
        fwrite(block, 4, 4, output_file);
    }

    fclose(output_file);
    fclose(source_file);
}

void decrypt(const char *encrypted_filename, const char *key_filename) {
    FILE *source_file = fopen(encrypted_filename, "rb");

    unsigned char trash_bytes_in_last_block_amount;

    fread(&trash_bytes_in_last_block_amount, sizeof(char), 1, source_file);

    FILE *output_file = fopen(DEFAULT_DECRYPTED_FILENAME, "wb");

    Key key;
    FILE *key_file = fopen(key_filename, "rb");
    fread(key.content, sizeof(WORD), 8, key_file);
    fclose(key_file);
    form_key(&key, KEY_LENGTH_BITS);

    BLOCK block;
    fread(block, sizeof(WORD), 4, source_file);
    BLOCK buffer;
    while (1) {
        int read_elements_amount = fread(buffer, sizeof(WORD), 4, source_file);
        if (read_elements_amount % 4 == 0 && read_elements_amount != 0) {
            decrypt_block(block, 128, block, key.sub_keys);
            fwrite(block, sizeof(WORD), 4, output_file);
            memcpy(block, buffer, sizeof(block));
        } else {
            decrypt_block(block, 128, block, key.sub_keys);
            fwrite(block, sizeof(char), 16 - trash_bytes_in_last_block_amount, output_file);
            break;
        }
    }

    fclose(output_file);
    fclose(source_file);
}


int main(int argc, char **argv) {
    srand(time(NULL));

    argc = 4;

    char *mode = "-d";
    char *input_filename = "encrypted.bin";
    char *key_filename;
    if (argc == 4)
        key_filename = "key.bin";

    if (argc == 3 && !strcmp(mode, "-e"))
        encrypt(input_filename);
    else if (argc == 4 && !strcmp(mode, "-d"))
        decrypt(input_filename, key_filename);
}