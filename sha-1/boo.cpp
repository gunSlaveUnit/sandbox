#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define u8 uint8_t
#define u32 uint32_t
#define u64 uint64_t

#define HASH_SIZE_BYTES 20
#define BLOCK_SIZE_BYTES 64

#define ROL(x, n) ((((x))«(n))| (((x))»(32-(n))))

u8 hash160_test[HASH_SIZE_BYTES] = {
        0x2f, 0xd4, 0xe1, 0xc6,
        0x7a, 0x2d, 0x28, 0xfc,
        0xed, 0x84, 0x9e, 0xe1,
        0xbb, 0x76, 0xe7, 0x39,
        0x1b, 0x93, 0xeb, 0x12
};

const u32 KEYS[4] = {
        0x5A827999,
        0x6ED9EBA1,
        0x8F1BBCDC,
        0xCA62C1D6
};

u32 H0 = 0x67452301;
u32 H1 = 0xEFCDAB89;
u32 H2 = 0x98BADCFE;
u32 H3 = 0x10325476;
u32 H4 = 0xC3D2E1F0;

inline static void buffer_to_block(const u8 *buffer, uint32_t block[16]) {
    for (size_t i = 0; i < 16; i++) {
        block[i] = (buffer[4 * i + 3] & 0xff)
                   | (buffer[4 * i + 2] & 0xff) « 8
                                                  | (buffer[4 * i + 1] & 0xff) « 16
                                                                                 | (buffer[4 * i + 0] & 0xff) « 24;
    }
}

void hsha(const u8 *yq) {
    u32 w[80];

    memcpy(w, yq, sizeof(u32) * 16);

    for (u8 t = 16; t < 80; ++t)
        w[t] = ROL(w[t - 3] ^ w[t - 8] ^ w[t - 14] ^ w[t - 16], 1);

    u32 A = H0;
    u32 B = H1;
    u32 C = H2;
    u32 D = H3;
    u32 E = H4;

    for (u8 t = 0; t < 80; ++t) {
        u32 F = 0, K = 0;
        if (t < 20) {
            F = ((B & C) | ((~B) & D));
            K = 0x5A827999;
        } else if (t < 40) {
            F = B ^ C ^ D;
            K = 0x6ED9EBA1;
        } else if (t < 60) {
            F = ((B & C) | (B & D) | (C & D));
            K = 0x8F1BBCDC;
        } else if (t < 80) {
            F = B ^ C ^ D;
            K = 0xCA62C1D6;
        }

        u32 temp = ROL(A, 5) + F + E + K + w[t];
        E = D;
        D = C;
        C = ROL(B, 30);
        B = A;
        A = temp;
    }

    H0 += A;
    H1 += B;
    H2 += C;
    H3 += D;
    H4 += E;
}

void sha_1(const u8 *message, u8 *out) {
    u32 length_bytes = strlen(message);
    u32 blocks_512_amount = length_bytes / 64;

    u8 block[BLOCK_SIZE_BYTES];
    for (u32 i = 0; i < blocks_512_amount; ++i) {
        memcpy(block, message + i * BLOCK_SIZE_BYTES, BLOCK_SIZE_BYTES);
        hsha(block);
    }

    u32 unaligned_block_length = length_bytes - blocks_512_amount * BLOCK_SIZE_BYTES;

    memset(block, 0x00, sizeof(u8) * BLOCK_SIZE_BYTES);
    memcpy(block, message + blocks_512_amount * BLOCK_SIZE_BYTES, length_bytes - blocks_512_amount * BLOCK_SIZE_BYTES);

    block[unaligned_block_length] = 0x80;

    u64 length_bits = length_bytes * 8;

    u32 *b = block;
    buffer_to_block(block, b);
    b[15] = (uint32_t) length_bits;
    b[14] = (uint32_t) (length_bits » 32);

    hsha((const uint8_t *) b);

    u64 *out_64 = (u64 *) out;
    out_64[0] = H0;
    out_64[1] = H1;
    out_64[2] = H2;
    out_64[3] = H3;
    out_64[4] = H4;
}

void print_block(const u8 *a, const u64 length_bytes) {
    for (u64 i = 0; i < length_bytes; ++i)
        printf("%hhx", a[i]);
}

int main() {
    const u8 message[] = "The quick brown fox jumps over the lazy dog";
    u8 hash[HASH_SIZE_BYTES];

    sha_1(message, hash);

    if (memcmp(hash, hash160_test, sizeof(u8) * HASH_SIZE_BYTES) == 0)
        printf("PASSED");
    else
        printf("FAILED");

    printf("\n");
    print_block(hash, HASH_SIZE_BYTES);
}