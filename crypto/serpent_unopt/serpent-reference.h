#ifndef _SERPENT_REFERENCE_
#define _SERPENT_REFERENCE_

#include "serpent-api.h"

#define ROL(x, n) ((((unsigned long)(x))<<(n))| \
                  (((unsigned long)(x))>>(32-(n))))
#define ROR(x, n) ((((unsigned long)(x))<<(32-(n)))| \
                  (((unsigned long)(x))>>(n)))

#define transform(x0, x1, x2, x3, y0, y1, y2, y3) \
      y0 = ROL(x0, 13); \
      y2 = ROL(x2, 3); \
      y1 = x1 ^ y0 ^ y2; \
      y3 = x3 ^ y2 ^ ((unsigned long)y0)<<3; \
      y1 = ROL(y1, 1); \
      y3 = ROL(y3, 7); \
      y0 = y0 ^ y1 ^ y3; \
      y2 = y2 ^ y3 ^ ((unsigned long)y1<<7); \
      y0 = ROL(y0, 5); \
      y2 = ROL(y2, 22)

#define inv_transform(x0, x1, x2, x3, y0, y1, y2, y3) \
      y2 = ROR(x2, 22);\
      y0 = ROR(x0, 5); \
      y2 = y2 ^ x3 ^ ((unsigned long)x1<<7); \
      y0 = y0 ^ x1 ^ x3; \
      y3 = ROR(x3, 7); \
      y1 = ROR(x1, 1); \
      y3 = y3 ^ y2 ^ ((unsigned long)y0)<<3; \
      y1 = y1 ^ y0 ^ y2; \
      y2 = ROR(y2, 3); \
      y0 = ROR(y0, 13)

void IP(BLOCK input, BLOCK output);
void FP(BLOCK input, BLOCK output);
void IPInverse(BLOCK output, BLOCK input);
void FPInverse(BLOCK output, BLOCK input);
void s_box_replacement(int box, BLOCK input, BLOCK output);
void s_box_replacement_inverse(int box, BLOCK output, BLOCK input);
void R(int i, BLOCK input, keySchedule sub_keys, BLOCK output);
void RInverse(int i, BLOCK BHatiPlus1, keySchedule KHat, BLOCK BHati);

void setBit(WORD x[], int p, BIT v);
BIT getBit(WORD x[], int p);
BIT getBitFromWord(WORD x, int p);
BIT getBitFromByte(BYTE x, int p);
BYTE getByte(WORD x, int p);
BYTE makeByte(BIT b0, BIT b1, BIT b2, BIT b3);
void key_mixing(BLOCK in1, BLOCK in2, BLOCK out);
void applyPermutation(permutationTable t, BLOCK input, BLOCK output);
BYTE S(int box, BYTE input);
BYTE SInverse(int box, BYTE output);

#endif
