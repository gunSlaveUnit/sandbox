#include "stribog.h"

void print_block(const u8 *a, const u64 length_bytes) {
    for (u64 i = 0; i < length_bytes; ++i)
        printf("%hhx", a[i]);
}

static void add_mod(const unsigned char *a, const unsigned char *b, unsigned char *c) {
    int overflow = 0;

    for (int i = 63; i >= 0; i--) {
        overflow = a[i] + b[i] + (overflow >> 8);
        c[i] = overflow & 0xFF;
    }
}

static void X(const void *a, const void *b, void *c) {
    const long long *x = a, *y = b;
    unsigned long long *z = c;

    for (int i = 0; i < 8; i++) {
        z[i] = x[i] ^ y[i];
    }
}

static void S(unsigned char *state) {
    for (u8 i = 0; i < 64; ++i)
        state[i] = Pi[state[i]];
}

static void L(unsigned char *state) {
    for (u8 i = 0; i < 8; i++) {
        u64 v = 0;
        for (u8 k = 0; k < 8; k++) {
            for (u8 j = 0; j < 8; j++) {
                if ((state[i * 8 + k] & (1 << (7 - j))))
                    v ^= A[k * 8 + j];
            }
        }
        for (u8 k = 0; k < 8; k++)
            state[i * 8 + k] = (v & ((u64) 0xFF << (7 - k) * 8)) >> (7 - k) * 8;
    }
}

static void P(unsigned char *state) {
    unsigned char t[64];

    for (int i = 0; i < 64; i++)
        t[i] = state[Tau[i]];

    memcpy(state, t, 64);
}

static void key_scheduling(unsigned char *K, int i) {
    X(K, C[i], K);

    S(K);
    P(K);
    L(K);
}

static void E(unsigned char *K, const unsigned char *m, unsigned char *state) {
    X(m, K, state);

    for (int i = 0; i < 12; i++) {
        S(state);
        P(state);
        L(state);
        key_scheduling(K, i);
        X(state, K, state);
    }
}

static void g_N(const unsigned char *N, unsigned char *h, const unsigned char *m) {
    unsigned char t[64], K[64];

    X(N, h, K);

    S(K);
    P(K);
    L(K);

    E(K, m, t);

    X(t, h, t);
    X(t, m, h);
}

void init_context(const u16 hash_size) {
    memset(&context, 0x00, sizeof(context));
    memset(context.h, hash_size == 512 ? 0 : 1, BLOCK_SIZE_BYTES);
    context.v_512[BLOCK_SIZE_BYTES - 2] = 0x02;
}

void stribog(const u8 *message, const u16 hash_size, u8 *out) {
    init_context(hash_size);

    u8 m[BLOCK_SIZE_BYTES];
    u64 length_bits = strlen(message) * 8;

    while (length_bits >= BLOCK_SIZE_BITS) {
        memcpy(m, message + length_bits / 8 - BLOCK_SIZE_BYTES, BLOCK_SIZE_BYTES);

        g_N(context.N, context.h, m);
        add_mod(context.N, context.v_512, context.N);
        add_mod(context.sigma, m, context.sigma);
        length_bits -= BLOCK_SIZE_BITS;
    }

    memset(m, 0, BLOCK_SIZE_BYTES);
    memcpy(m + BLOCK_SIZE_BYTES - length_bits / 8, message, length_bits / 8);

    m[63 - length_bits / 8] |= 1;

    g_N(context.N, context.h, m);
    context.v_512[63] = length_bits & 0xFF;
    context.v_512[62] = length_bits >> 8;
    add_mod(context.N, context.v_512, context.N);

    add_mod(context.sigma, m, context.sigma);

    g_N(context.v_0, context.h, context.N);
    g_N(context.v_0, context.h, context.sigma);

    memcpy(out, context.h, hash_size == 512 ? BLOCK_SIZE_BYTES : HALF_BLOCK_SIZE_BYTES);
}
