#pragma once

#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

#define i8 int8_t
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define BLOCK_SIZE_BYTES 64
#define HALF_BLOCK_SIZE_BYTES (BLOCK_SIZE_BYTES / 2)
#define BLOCK_SIZE_BITS (BLOCK_SIZE_BYTES * 8)

typedef u8 BLOCK[BLOCK_SIZE_BYTES];

static struct Context {
    BLOCK h;
    BLOCK N;
    BLOCK sigma;
    BLOCK v_0;
    BLOCK v_512;
} context;

#endif