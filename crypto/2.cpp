#include <bitset>
#include <cstdint>
#include <ctime>
#include <fstream>
#include <iostream>
#include <optional>
#include <vector>

using u8 = uint_fast8_t;
using u32 = uint_fast32_t;

using u1 = u8;

class BitReader {
    std::ifstream file_reader;
    u8 curr_byte;
    u8 curr_pos;

public:
    BitReader(std::ifstream &&file_reader)
            : file_reader(std::move(file_reader)) {}

    ~BitReader() { file_reader.close(); }

    std::optional<u1> read_bit() {
        if (this->curr_pos % 8 == 0) {
            if (!file_reader.get((char &) curr_byte))
                return {};
            curr_pos = 0;
        }
        return static_cast<u1>(this->curr_byte >> (7 - this->curr_pos++) & 0b1);
    }

    std::optional<u8> read_byte() {
        auto res = read_bit();
        if (!res)
            return {};
        for (auto i = 0; i < 7; i++) {
            auto b = read_bit();
            if (!b)
                return res;
            res = *res << 1 | *b;
        }
        return res;
    }
};

class BitWriter {
    std::ofstream file_writer;
    u8 curr_byte;
    u8 curr_pos;

public:
    BitWriter(std::ofstream &&file_writer)
            : file_writer(std::move(file_writer)), curr_byte(0), curr_pos(0) {}

    ~BitWriter() {
        if (curr_pos % 8 != 0) {
            curr_byte <<= 8 - curr_pos;
            file_writer.write((char *) &curr_byte, 1);
        }
        file_writer.flush();
        file_writer.close();
    }

    void write_bit(u1 bit) {
        curr_byte = curr_byte << 1 | bit & 0b1;
        if (++curr_pos == 8) {
            file_writer.write((char *) &curr_byte, 1);
            curr_pos = 0;
        }
    }

    void write_byte(u8 byte) {
        for (auto i = 0; i < 8; i++)
            write_bit(byte >> (7 - i));
    }
};

constexpr u32 KEY_LENGTH_BYTES = 32;
constexpr u32 KEY_LENGTH_BITS = KEY_LENGTH_BYTES << 3;
constexpr u32 KEY_VALUE_UPPER_BOUND = 256;

void prepare_key(u32 length) {
    auto key = BitWriter(std::ofstream("key.txt", std::ios::binary));

    for (auto j = 0; j < length + 3; j++) {
        key.write_byte(rand());
    }
}

u1 bit_xor_sum(u8 byte) {
    u8 sum_bit = byte;
    for (auto u = 0; u < 8; ++u)
        sum_bit = (sum_bit >> 1) ^ (sum_bit & 1);
    return sum_bit;
}

void encrypt(u8 sync_message,
             std::ifstream &&key_file,
             std::ifstream &&input_file,
             std::ofstream &&output_file) {
    auto key = BitReader(std::move(key_file));
    auto input = BitReader(std::move(input_file));
    auto output = BitWriter(std::move(output_file));

    auto padding = 8;

// 0.
    auto b = sync_message;
    auto k = key.read_byte().value_or(0);

    while (true) {
// 1.
        auto m = b ^ k;

// 2.
        auto t = bit_xor_sum(m);

// 3.
        t ^= ({
            auto opt = input.read_bit();
            if (!opt && padding-- == 0)
                return;
            opt.value_or(0);
        });

// 4.
        output.write_bit(b >> 7);
        b <<= 1;
        k <<= 1;

// 5.
        b |= t;
        k |= key.read_bit().value_or(0);
    }
}

void decrypt(std::ifstream &&key_file,
             std::ifstream &&input_file,
             std::ofstream &&output_file) {
    auto key = BitReader(std::move(key_file));
    auto input = BitReader(std::move(input_file));
    auto output = BitWriter(std::move(output_file));

    auto b = input.read_byte().value_or(0);
    auto k = key.read_byte().value_or(0);

    while (true) {
// 1.
        auto m = b ^ k;

// 2.
        auto t = bit_xor_sum(m);

// 3.
        auto h = ({
            auto opt = input.read_bit();
            if (!opt)
                return;
            *opt;
        });
        t ^= h;

// 4.
        output.write_bit(t);
        b <<= 1;


        k <<= 1;

// 5.
        b |= h;
        k |= key.read_bit().value_or(0);
    }
}

int main() {
    srand(time(nullptr));

// auto sync_message = rand() % KEY_VALUE_UPPER_BOUND;
    auto sync_message = 0b10010000;

    auto filesize = ({
        std::ifstream input("alice.txt", std::ios::binary | std::ifstream::ate);
        input.tellg();
    });

    prepare_key(filesize);
    encrypt(sync_message, std::ifstream("key.txt", std::ios::binary),
            std::ifstream("alice.txt", std::ios::binary),
            std::ofstream("encrypted.txt", std::ios::binary));
    decrypt(std::ifstream("key.txt", std::ios::binary),
            std::ifstream("encrypted.txt", std::ios::binary),
            std::ofstream("decrypted.txt", std::ios::binary));
}