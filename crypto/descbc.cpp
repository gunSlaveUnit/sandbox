#include <iostream>
#include <cstdint>
#include <fstream>

#define LB_MASK_32 0x00000001
#define LB_MASK_64 0x0000000000000001
#define L_MASK_64  0x00000000ffffffff

using u8 = uint_fast8_t;
using u32 = uint_fast32_t;
using u64 = uint_fast64_t;

const u8 r = 16;
u64 sub_keys[r];

const u8 IP[]{
        58, 50, 42, 34, 26, 18, 10, 2,
        60, 52, 44, 36, 28, 20, 12, 4,
        62, 54, 46, 38, 30, 22, 14, 6,
        64, 56, 48, 40, 32, 24, 16, 8,
        57, 49, 41, 33, 25, 17, 9, 1,
        59, 51, 43, 35, 27, 19, 11, 3,
        61, 53, 45, 37, 29, 21, 13, 5,
        63, 55, 47, 39, 31, 23, 15, 7
};

const u8 FP[]{
        40, 8, 48, 16, 56, 24, 64, 32,
        39, 7, 47, 15, 55, 23, 63, 31,
        38, 6, 46, 14, 54, 22, 62, 30,
        37, 5, 45, 13, 53, 21, 61, 29,
        36, 4, 44, 12, 52, 20, 60, 28,
        35, 3, 43, 11, 51, 19, 59, 27,
        34, 2, 42, 10, 50, 18, 58, 26,
        33, 1, 41, 9, 49, 17, 57, 25
};

const u8 PC1[]{
        57, 49, 41, 33, 25, 17, 9,
        1, 58, 50, 42, 34, 26, 18,
        10, 2, 59, 51, 43, 35, 27,
        19, 11, 3, 60, 52, 44, 36,
        63, 55, 47, 39, 31, 23, 15,
        7, 62, 54, 46, 38, 30, 22,
        14, 6, 61, 53, 45, 37, 29,
        21, 13, 5, 28, 20, 12, 4
};

const u8 PC2[]{
        14, 17, 11, 24, 1, 5,
        3, 28, 15, 6, 21, 10,
        23, 19, 12, 4, 26, 8,
        16, 7, 27, 20, 13, 2,
        41, 52, 31, 37, 47, 55,
        30, 40, 51, 45, 33, 48,
        44, 49, 39, 56, 34, 53,
        46, 42, 50, 36, 29, 32
};

const u8 SHIFTS[]{
        1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

const u8 EXPANSION[]{
        32, 1, 2, 3, 4, 5,
        4, 5, 6, 7, 8, 9,
        8, 9, 10, 11, 12, 13,
        12, 13, 14, 15, 16, 17,
        16, 17, 18, 19, 20, 21,
        20, 21, 22, 23, 24, 25,
        24, 25, 26, 27, 28, 29,
        28, 29, 30, 31, 32, 1
};

const u8 S_BOXES[8][64]{
        {
                14, 4,  13, 1,  2,  15, 11, 8,  3,  10, 6,  12, 5,  9,  0,  7,
                0,  15, 7,  4,  14, 2,  13, 1,  10, 6, 12, 11, 9,  5,  3,  8,
                4,  1,  14, 8,  13, 6,  2,  11, 15, 12, 9,  7,  3,  10, 5,  0,
                15, 12, 8,  2,  4,  9,  1,  7,  5,  11, 3,  14, 10, 0, 6,  13
        },
        {
                15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
                3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
                0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
                13, 8, 10, 1, 3, 15, 4, 2, 11, 6,       7,  12, 0,  5, 14, 9
        },
        {
                10, 0,  9,  14, 6,  3,  15, 5,  1,  13, 12, 7,  11, 4,  2,  8,
                13, 7,  0,  9,  3,  4,  6,  10, 2,  8, 5,  14, 12, 11, 15, 1,
                13, 6,  4,  9,  8,  15, 3,  0,  11, 1,  2,  12, 5,  10, 14, 7,
                1,  10, 13, 0,  6,  9,  8,  7,  4,  15, 14, 3,  11, 5, 2,  12
        },
        {
                7,  13, 14, 3,  0,  6,  9,  10, 1,  2,  8,  5,  11, 12, 4,  15,
                13, 8,  11, 5,  6,  15, 0,  3,  4,  7, 2,  12, 1,  10, 14, 9,
                10, 6,  9,  0,  12, 11, 7,  13, 15, 1,  3,  14, 5,  2,  8,  4,
                3,  15, 0,  6,  10, 1,  13, 8,  9,  4,  5,  11, 12, 7, 2,  14
        },
        {
                2,  12, 4,  1,  7,  10, 11, 6,  8,  5,  3,  15, 13, 0,  14, 9,
                14, 11, 2,  12, 4,  7,  13, 1,  5,  0, 15, 10, 3,  9,  8,  6,
                4,  2,  1,  11, 10, 13, 7,  8,  15, 9,  12, 5,  6,  3,  0,  14,
                11, 8,  12, 7,  1,  14, 2,  13, 6,  15, 0,  9,  10, 4, 5,  3
        },
        {
                12, 1,  10, 15, 9,  2,  6,  8,  0,  13, 3,  4,  14, 7,  5,  11,
                10, 15, 4,  2,  7,  12, 9,  5,  6,  1, 13, 14, 0,  11, 3,  8,
                9,  14, 15, 5,  2,  8,  12, 3,  7,  0,  4,  10, 1,  13, 11, 6,
                4,  3,  2,  12, 9,  5,  15, 10, 11, 14, 1,  7,  6,  0, 8,  13
        },
        {
                4,  11, 2,  14, 15, 0,  8,  13, 3,  12, 9,  7,  5,  10, 6,  1,
                13, 0,  11, 7,  4,  9,  1,  10, 14, 3, 5,  12, 2,  15, 8,  6,
                1,  4,  11, 13, 12, 3,  7,  14, 10, 15, 6,  8,  0,  5,  9,  2,
                6,  11, 13, 8,  1,  4,  10, 7,  9,  5,  0,  15, 14, 2, 3,  12
        },
        {
                13, 2,  8,  4,  6,  15, 11, 1,  10, 9,  3,  14, 5,  0,  12, 7,
                1,  15, 13, 8,  10, 3,  7,  4,  12, 5, 6,  11, 0,  14, 9,  2,
                7,  11, 4,  1,  9,  12, 14, 2,  0,  6,  10, 13, 15, 3,  5,  8,
                2,  1,  14, 7,  4,  10, 8,  13, 15, 12, 9,  0,  3,  5, 6,  11
        }
};

const u8 POST_S_BOX[]{
        16, 7, 20, 21,
        29, 12, 28, 17,
        1, 15, 23, 26,
        5, 18, 31, 10,
        2, 8, 24, 14,
        32, 27, 3, 9,
        19, 13, 30, 6,
        22, 11, 4, 25
};

void key_scheduling(u64 key) {
    u64 key_pc1 = 0;
    for (u8 i = 0; i < 56; ++i) {
        key_pc1 <<= 1;
        key_pc1 |= (key >> (64 - PC1[i])) & LB_MASK_64;
    }

    u32 C = (u32) ((key_pc1 >> 28) & 0x000000000fffffff);
    u32 D = (u32) (key_pc1 & 0x000000000fffffff);

    for (u8 i = 0; i < r; ++i) {
        for (u8 j = 0; j < SHIFTS[i]; j++) {
            C = (0x0fffffff & (C << 1)) | (0x00000001 & (C >> 27));
            D = (0x0fffffff & (D << 1)) | (0x00000001 & (D >> 27));
        }

        u64 permuted_choice_2 = (((u64) C) << 28) | (u64) D;

        sub_keys[i] = 0;
        for (u8 j = 0; j < 48; j++) {
            sub_keys[i] <<= 1;
            sub_keys[i] |= (permuted_choice_2 >> (56 - PC2[j])) & LB_MASK_64;
        }
    }
}

u64 f(u32 R, u64 sub_key) {
    u64 s_input = 0;
    for (u8 i = 0; i < 48; i++) {
        s_input <<= 1;
        s_input |= (u64) ((R >> (32 - EXPANSION[i])) & LB_MASK_32);
    }

    s_input = s_input ^ sub_key;

    u32 s_output = 0;
    for (u8 i = 0; i < 8; i++) {
        char row = (char) ((s_input & (0x0000840000000000 >> 6 * i)) >> (42 - 6 * i));
        row = (row >> 4) | (row & 0x01);

        // Middle 4 bits of input
        char column = (char) ((s_input & (0x0000780000000000 >> 6 * i)) >> (43 - 6 * i));

        s_output <<= 4;
        s_output |= (u32) (S_BOXES[i][16 * row + column] & 0x0f);
    }

    // applying the round permutation
    u32 f_result = 0;
    for (u8 i = 0; i < 32; i++) {
        f_result <<= 1;
        f_result |= (s_output >> (32 - POST_S_BOX[i])) & LB_MASK_32;
    }

    return f_result;
}

u64 ip(u64 block) {
    u64 result = 0;
    for (u8 i: IP) {
        result <<= 1;
        result |= (block >> (64 - i)) & LB_MASK_64;
    }
    return result;
}

u64 fp(u64 block) {
    u64 result = 0;
    for (u8 i: FP) {
        result <<= 1;
        result |= (block >> (64 - i)) & LB_MASK_64;
    }
    return result;
}

u64 encrypt_block(u64 block) {
    block = ip(block);

    u32 L = (u32) (block >> 32) & L_MASK_64;
    u32 R = (u32) (block & L_MASK_64);

    for (u64 sub_key: sub_keys) {
        u32 V = f(R, sub_key);

        u32 temp = R;
        R = L ^ V;
        L = temp;
    }

    block = (((u64) R) << 32) | (u64) L;

    return fp(block);
}

u64 decrypt_block(u64 block) {
    block = ip(block);

    u32 L = (u32) (block >> 32) & L_MASK_64;
    u32 R = (u32) (block & L_MASK_64);

    for (u8 i = 0; i < 16; ++i) {
        u32 V = f(R, sub_keys[15 - i]);

        u32 temp = R;
        R = L ^ V;
        L = temp;
    }

    block = (((u64) R) << 32) | (u64) L;

    return fp(block);
}

void encrypt(const std::string &source_filename,
             const std::string &output_filename) {
    std::ifstream input_file(source_filename, std::ios::binary);
    std::ofstream output_file(output_filename, std::ios::binary);

    u64 block, iv = rand(), last_block = iv;
    output_file.write((char *) &iv, 8);
    while (input_file.read((char *) &block, 8)) {
        last_block = encrypt_block(block ^ last_block);
        output_file.write((char *) &last_block, 8);
    }

    output_file.flush();
    output_file.close();
    input_file.close();
}

void decrypt(const std::string &source_filename,
             const std::string &output_filename) {
    std::ifstream input_file(source_filename, std::ios::binary);
    std::ofstream output_file(output_filename, std::ios::binary);

    u64 iv = 0;
    input_file.read((char *) &iv, 8);
    u64 block, last_block = iv, result;
    while (input_file.read((char *) &block, 8)) {
        result = decrypt_block(block) ^ last_block;
        last_block = block;
        output_file.write((char *) &result, 8);
    }

    output_file.flush();
    output_file.close();
    input_file.close();
}

int main() {
    u64 key = 0;
    key_scheduling(key);

    encrypt("../war.txt", "../encrypted.bin");
    decrypt("../encrypted.bin", "../decrypted.txt");
}