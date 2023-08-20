#ifndef _SERPENT_API_
#define _SERPENT_API_



#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_XOR_TAPS_PER_BIT 7

#define r 32
#define phi 0x9e3779b9

#define BITS_PER_BYTE 8
#define BITS_PER_NIBBLE 4
#define BYTES_PER_WORD 4
#define WORDS_PER_BLOCK 4
#define WORDS_PER_KEY 8

#define NIBBLES_PER_WORD (BITS_PER_WORD/BITS_PER_NIBBLE)
#define BITS_PER_WORD (BITS_PER_BYTE*BYTES_PER_WORD)
#define BITS_PER_BLOCK (BITS_PER_WORD*WORDS_PER_BLOCK)

typedef unsigned char BYTE;
typedef unsigned long WORD;

typedef unsigned char BIT;

typedef int permutationTable[BITS_PER_BLOCK];
typedef BYTE xorTable[BITS_PER_BLOCK][MAX_XOR_TAPS_PER_BIT+1];

typedef WORD keySchedule[r+1][WORDS_PER_BLOCK];
typedef WORD BLOCK[WORDS_PER_BLOCK];
typedef WORD KEY[WORDS_PER_KEY];

#endif