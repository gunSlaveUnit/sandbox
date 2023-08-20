#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "serpent.h"

#define ROL(x, n) ((((unsigned long)(x))<<(n))| \
                  (((unsigned long)(x))>>(32-(n))))
#define ROR(x, n) ((((unsigned long)(x))<<(32-(n)))| \
                  (((unsigned long)(x))>>(n)))

typedef unsigned char byte;
typedef unsigned int dword;

typedef struct {
    dword content[8];
    dword sub_keys[33][4];
} Key;

const char *DEFAULT_ENCRYPTED_FILENAME = "encrypted.bin";
const char *DEFAULT_DECRYPTED_FILENAME = "decrypted.txt";
const char *DEFAULT_KEY_FILENAME = "key.bin";
const int KEY_LENGTH_BITS = 256;
const int BLOCK_LENGTH_BITS = 128;

void form_key(Key *key, int length) {
    dword w[132], k[132];
    for (int i = 0; i < length / 32; ++i)
        w[i] = key->content[i];
    for (int i = 8; i < 16; ++i)
        w[i] = ROL(w[i - 8] ^ w[i - 5] ^ w[i - 3] ^ w[i - 1] ^ PHI ^ (i - 8), 11);
    for (int i = 0; i < 8; ++i)
        w[i] = w[i + 8];
    for (int i = 8; i < 132; ++i)
        w[i] = ROL(w[i - 8] ^ w[i - 5] ^ w[i - 3] ^ w[i - 1] ^ PHI ^ i, 11);

    RND03(w[0], w[1], w[2], w[3], k[0], k[1], k[2], k[3]);
    RND02(w[4], w[5], w[6], w[7], k[4], k[5], k[6], k[7]);
    RND01(w[8], w[9], w[10], w[11], k[8], k[9], k[10], k[11]);
    RND00(w[12], w[13], w[14], w[15], k[12], k[13], k[14], k[15]);
    RND31(w[16], w[17], w[18], w[19], k[16], k[17], k[18], k[19]);
    RND30(w[20], w[21], w[22], w[23], k[20], k[21], k[22], k[23]);
    RND29(w[24], w[25], w[26], w[27], k[24], k[25], k[26], k[27]);
    RND28(w[28], w[29], w[30], w[31], k[28], k[29], k[30], k[31]);
    RND27(w[32], w[33], w[34], w[35], k[32], k[33], k[34], k[35]);
    RND26(w[36], w[37], w[38], w[39], k[36], k[37], k[38], k[39]);
    RND25(w[40], w[41], w[42], w[43], k[40], k[41], k[42], k[43]);
    RND24(w[44], w[45], w[46], w[47], k[44], k[45], k[46], k[47]);
    RND23(w[48], w[49], w[50], w[51], k[48], k[49], k[50], k[51]);
    RND22(w[52], w[53], w[54], w[55], k[52], k[53], k[54], k[55]);
    RND21(w[56], w[57], w[58], w[59], k[56], k[57], k[58], k[59]);
    RND20(w[60], w[61], w[62], w[63], k[60], k[61], k[62], k[63]);
    RND19(w[64], w[65], w[66], w[67], k[64], k[65], k[66], k[67]);
    RND18(w[68], w[69], w[70], w[71], k[68], k[69], k[70], k[71]);
    RND17(w[72], w[73], w[74], w[75], k[72], k[73], k[74], k[75]);
    RND16(w[76], w[77], w[78], w[79], k[76], k[77], k[78], k[79]);
    RND15(w[80], w[81], w[82], w[83], k[80], k[81], k[82], k[83]);
    RND14(w[84], w[85], w[86], w[87], k[84], k[85], k[86], k[87]);
    RND13(w[88], w[89], w[90], w[91], k[88], k[89], k[90], k[91]);
    RND12(w[92], w[93], w[94], w[95], k[92], k[93], k[94], k[95]);
    RND11(w[96], w[97], w[98], w[99], k[96], k[97], k[98], k[99]);
    RND10(w[100], w[101], w[102], w[103], k[100], k[101], k[102], k[103]);
    RND09(w[104], w[105], w[106], w[107], k[104], k[105], k[106], k[107]);
    RND08(w[108], w[109], w[110], w[111], k[108], k[109], k[110], k[111]);
    RND07(w[112], w[113], w[114], w[115], k[112], k[113], k[114], k[115]);
    RND06(w[116], w[117], w[118], w[119], k[116], k[117], k[118], k[119]);
    RND05(w[120], w[121], w[122], w[123], k[120], k[121], k[122], k[123]);
    RND04(w[124], w[125], w[126], w[127], k[124], k[125], k[126], k[127]);
    RND03(w[128], w[129], w[130], w[131], k[128], k[129], k[130], k[131]);

    for (int i = 0; i < 33; ++i)
        for (int j = 0; j < 4; ++j)
            key->sub_keys[i][j] = k[4 * i + j];
}

void encrypt_block(const dword *input, int input_length, dword *output, const dword sub_keys[33][4]) {
    for (int b = 0; b < input_length; b += BLOCK_LENGTH_BITS, input += 16, output += 16) {
        register unsigned long x0, x1, x2, x3;
        register unsigned long y0, y1, y2, y3;

        x0 = input[0];
        x1 = input[1];
        x2 = input[2];
        x3 = input[3];

        keying(x0, x1, x2, x3, sub_keys[0]);
        RND00(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[1]);
        RND01(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[2]);
        RND02(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[3]);
        RND03(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[4]);
        RND04(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[5]);
        RND05(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[6]);
        RND06(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[7]);
        RND07(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[8]);
        RND08(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[9]);
        RND09(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[10]);
        RND10(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[11]);
        RND11(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[12]);
        RND12(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[13]);
        RND13(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[14]);
        RND14(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[15]);
        RND15(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[16]);
        RND16(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[17]);
        RND17(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[18]);
        RND18(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[19]);
        RND19(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[20]);
        RND20(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[21]);
        RND21(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[22]);
        RND22(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[23]);
        RND23(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[24]);
        RND24(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[25]);
        RND25(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[26]);
        RND26(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[27]);
        RND27(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[28]);
        RND28(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[29]);
        RND29(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[30]);
        RND30(x0, x1, x2, x3, y0, y1, y2, y3);
        transform(y0, y1, y2, y3, x0, x1, x2, x3);
        keying(x0, x1, x2, x3, sub_keys[31]);
        RND31(x0, x1, x2, x3, y0, y1, y2, y3);
        x0 = y0;
        x1 = y1;
        x2 = y2;
        x3 = y3;
        keying(x0, x1, x2, x3, sub_keys[32]);

        output[0] = x0;
        output[1] = x1;
        output[2] = x2;
        output[3] = x3;
    }
}

void decrypt_block(const dword *input, int input_length, dword *output, const dword sub_keys[33][4]) {
    for (int b = 0; b < input_length; b += BLOCK_LENGTH_BITS, input += 16, output += 16) {
        register unsigned long x0, x1, x2, x3;
        register unsigned long y0, y1, y2, y3;

        x0 = input[0];
        x1 = input[1];
        x2 = input[2];
        x3 = input[3];

        keying(x0, x1, x2, x3, sub_keys[32]);
        InvRND31(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[31]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND30(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[30]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND29(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[29]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND28(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[28]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND27(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[27]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND26(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[26]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND25(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[25]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND24(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[24]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND23(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[23]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND22(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[22]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND21(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[21]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND20(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[20]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND19(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[19]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND18(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[18]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND17(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[17]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND16(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[16]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND15(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[15]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND14(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[14]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND13(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[13]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND12(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[12]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND11(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[11]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND10(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[10]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND09(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[9]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND08(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[8]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND07(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[7]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND06(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[6]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND05(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[5]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND04(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[4]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND03(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[3]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND02(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[2]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND01(x0, x1, x2, x3, y0, y1, y2, y3);
        keying(y0, y1, y2, y3, sub_keys[1]);
        inv_transform(y0, y1, y2, y3, x0, x1, x2, x3);
        InvRND00(x0, x1, x2, x3, y0, y1, y2, y3);
        x0 = y0;
        x1 = y1;
        x2 = y2;
        x3 = y3;
        keying(x0, x1, x2, x3, sub_keys[0]);

        output[0] = x0;
        output[1] = x1;
        output[2] = x2;
        output[3] = x3;
    }
}

void encrypt(const char *source_filename) {
    FILE *source_file = fopen(source_filename, "rb");
    FILE *output_file = fopen(DEFAULT_ENCRYPTED_FILENAME, "wb");

    Key key;
    for (int i = 0; i < 8; ++i)
        key.content[i] = rand();

    form_key(&key, KEY_LENGTH_BITS);
    FILE *key_file = fopen(DEFAULT_KEY_FILENAME, "wb");
    fwrite(key.content, sizeof(dword), 8, key_file);
    fclose(key_file);

    dword block[4];
    while (fread(block, 4, 4, source_file)) {
        encrypt_block(block, 128, block, key.sub_keys);
        fwrite(block, 4, 4, output_file);
    }

    fclose(output_file);
    fclose(source_file);
}

void decrypt(const char *encrypted_filename, const char *key_filename) {
    FILE *source_file = fopen(encrypted_filename, "rb");
    FILE *output_file = fopen(DEFAULT_DECRYPTED_FILENAME, "wb");

    Key key;
    FILE *key_file = fopen(key_filename, "rb");
    fread(key.content, sizeof(dword), 8, key_file);
    fclose(key_file);
    form_key(&key, KEY_LENGTH_BITS);

    dword block[4];
    while (fread(block, sizeof(dword), 4, source_file)) {
        decrypt_block(block, 128, block, key.sub_keys);
        fwrite(block, sizeof(dword), 4, output_file);
    }

    fclose(output_file);
    fclose(source_file);
}


int main(int argc, char **argv) {
    srand(time(NULL));

    argc = 3;

    char *mode = "-e";
    char *input_filename = "alice.txt";
    char *key_filename;
    if (argc == 4)
        key_filename = "key.bin";

    if (argc == 3 && !strcmp(mode, "-e"))
        encrypt(input_filename);
    else if (argc == 4 && !strcmp(mode, "-d"))
        decrypt(input_filename, key_filename);
}