#include <cstdint>
#include <cstdio>
#include <cstring>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>

#define u8 uint8_t

#define HASH_SIZE_BYTES 20


unsigned int ROTL(unsigned int a, int n) {
    unsigned int t = (a << n) | (a >> (32 - n));
    return t;
}

std::string pad_message(unsigned char *message, unsigned long long length) {
    const unsigned int block_size = 64;
    const unsigned int padding_size = block_size - ((length + 8) % block_size);
    const unsigned int total_size = length + padding_size + 8;

    unsigned char* padded_message = new unsigned char[total_size];
    std::memset(padded_message, 0, total_size);
    std::memcpy(padded_message, message, length);
    padded_message[length] = 0x80;

    uint64_t bits_length = length * 8;
    char buffer[sizeof(uint64_t)];
    buffer[0] = (bits_length >> 56) & 0xFF;
    buffer[1] = (bits_length >> 48) & 0xFF;
    buffer[2] = (bits_length >> 40) & 0xFF;
    buffer[3] = (bits_length >> 32) & 0xFF;
    buffer[4] = (bits_length >> 24) & 0xFF;
    buffer[5] = (bits_length >> 16) & 0xFF;
    buffer[6] = (bits_length >> 8) & 0xFF;
    buffer[7] = bits_length & 0xFF;
    padded_message[total_size - 8] = buffer[0];
    padded_message[total_size - 7] = buffer[1];
    padded_message[total_size - 6] = buffer[2];
    padded_message[total_size - 5] = buffer[3];
    padded_message[total_size - 4] = buffer[4];
    padded_message[total_size - 3] = buffer[5];
    padded_message[total_size - 2] = buffer[6];
    padded_message[total_size - 1] = buffer[7];

    std::string result(reinterpret_cast<char*>(padded_message), total_size);
    delete[] padded_message;
    return result;
}

std::string SHA1(const std::string &message) {
    std::vector<uint32_t> H = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};

    uint32_t le = message.length();

    std::string padded_message = pad_message((unsigned char *) message.c_str(), le);

    for (size_t i = 0; i < padded_message.size(); i += 64) {
        std::string chunk = padded_message.substr(i, 64);
        std::vector<uint32_t> W(80);

        for (size_t t = 0; t < 16; ++t) {
            W[t] = (chunk[4 * t] & 0xff) << 24;
            W[t] |= (chunk[4 * t + 1] & 0xff) << 16;
            W[t] |= (chunk[4 * t + 2] & 0xff) << 8;
            W[t] |= chunk[4 * t + 3] & 0xff;
        }

        for (size_t t = 16; t < 80; ++t) {
            W[t] = ROTL(W[t - 3] ^ W[t - 8] ^ W[t - 14] ^ W[t - 16], 1);
        }

        uint32_t A = H[0];
        uint32_t B = H[1];
        uint32_t C = H[2];
        uint32_t D = H[3];
        uint32_t E = H[4];

        for (size_t t = 0; t < 80; ++t) {
            uint32_t f, k;
            if (t < 20) {
                f = (B & C) | ((~B) & D);
                k = 0x5A827999;
            } else if (t < 40) {
                f = B ^ C ^ D;
                k = 0x6ED9EBA1;
            } else if (t < 60) {
                f = (B & C) | (B & D) | (C & D);
                k = 0x8F1BBCDC;
            } else {
                f = B ^ C ^ D;
                k = 0xCA62C1D6;
            }

            uint32_t temp = ROTL(A, 5) + f + E + k + W[t];
            E = D;
            D = C;
            C = ROTL(B, 30);
            B = A;
            A = temp;
        }

        H[0] += A;
        H[1] += B;
        H[2] += C;
        H[3] += D;
        H[4] += E;
    }

    std::stringstream result;
    for (uint32_t h: H) {
        result << std::setfill('0') << std::setw(8) << std::hex << h;
    }
    return result.str();
}

int main() {
    const std::string message = "The quick brown fox jumps over the lazy dog";
    u8 hash[HASH_SIZE_BYTES];

    std::cout << SHA1(message);
}