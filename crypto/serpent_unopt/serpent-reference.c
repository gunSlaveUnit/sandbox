#include "serpent-api.h"
#include "serpent-tables.h"
#include "serpent-reference.h"

void setBit(WORD x[], int p, BIT v) {
    if (v)
        x[p / BITS_PER_WORD] |= ((WORD) 0x1 << p % BITS_PER_WORD);
    else
        x[p / BITS_PER_WORD] &= ~((WORD) 0x1 << p % BITS_PER_WORD);
}

BIT getBit(WORD x[], int p) {

    return (BIT) ((x[p / BITS_PER_WORD]
                   & ((WORD) 0x1 << p % BITS_PER_WORD)) >> p % BITS_PER_WORD);
}

BIT getBitFromWord(WORD x, int p) {
    return (BIT) ((x & ((WORD) 0x1 << p)) >> p);
}

BIT getBitFromByte(BYTE x, int p) {
    return (BIT) ((x & (0x1 << p)) >> p);
}

BYTE getByte(WORD x, int p) {
    return (BYTE) (0xf & (x >> (p * BITS_PER_NIBBLE)));
}

BYTE makeByte(BIT b0, BIT b1, BIT b2, BIT b3) {
    return (BYTE) (b0 | (b1 << 1) | (b2 << 2) | (b3 << 3));
}

void key_mixing(BLOCK in1, BLOCK in2, BLOCK out) {
    for (int i = 0; i < WORDS_PER_BLOCK; ++i)
        out[i] = in1[i] ^ in2[i];
}

void applyPermutation(permutationTable t, BLOCK input, BLOCK output) {
    for (int p = 0; p < WORDS_PER_BLOCK; p++)
        output[p] = 0;

    for (int p = 0; p < BITS_PER_BLOCK; p++)
        setBit(output, p, getBit(input, t[p]));
}

BYTE S(int box, BYTE input) {
    return SBox[box][input];
}

BYTE SInverse(int box, BYTE output) {
    return SBoxInverse[box][output];
}

void IP(BLOCK input, BLOCK output) {
    applyPermutation(IPTable, input, output);
}

void FP(BLOCK input, BLOCK output) {
    applyPermutation(FPTable, input, output);
}

void IPInverse(BLOCK output, BLOCK input) {
    applyPermutation(FPTable, output, input);
}

void FPInverse(BLOCK output, BLOCK input) {
    applyPermutation(IPTable, output, input);
}

void s_box_replacement(int box, BLOCK input, BLOCK output) {
    for (int iWord = 0; iWord < WORDS_PER_BLOCK; iWord++) {
        output[iWord] = 0;
        for (int iNibble = 0; iNibble < NIBBLES_PER_WORD; iNibble++) {
            output[iWord] |= ((WORD) S(box, getByte(input[iWord], iNibble)))
                    << (iNibble * BITS_PER_NIBBLE);
        }
    }
}

void s_box_replacement_inverse(int box, BLOCK output, BLOCK input) {
    for (int iWord = 0; iWord < WORDS_PER_BLOCK; iWord++) {
        input[iWord] = 0;
        for (int iNibble = 0; iNibble < NIBBLES_PER_WORD; iNibble++) {
            input[iWord] |= ((WORD) SInverse(box, getByte(output[iWord], iNibble)))
                    << (iNibble * BITS_PER_NIBBLE);
        }
    }
}

void R(int i, BLOCK input, keySchedule sub_keys, BLOCK output) {
    BLOCK mixed, replaced;

    key_mixing(input, sub_keys[i], mixed);
    s_box_replacement(i % 8, mixed, replaced);

    if (i == r - 1)
        key_mixing(replaced, sub_keys[r], output);
    else {
        transform(replaced[0], replaced[1], replaced[2], replaced[3], output[0], output[1], output[2], output[3]);
    }
}

void RInverse(int i, BLOCK input, keySchedule sub_keys, BLOCK output) {
    BLOCK mixed, replaced;

    if (i == r - 1)
        key_mixing(input, sub_keys[r], replaced);
    else {
        inv_transform(input[0], input[1], input[2], input[3], replaced[0], replaced[1], replaced[2], replaced[3]);
    }

    s_box_replacement_inverse(i % 8, replaced, mixed);
    key_mixing(mixed, sub_keys[i], output);
}