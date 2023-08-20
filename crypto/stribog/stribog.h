#pragma once

#ifndef STRIBOG_H
#define STRIBOG_H

#include <stdio.h>
#include <memory.h>
#include <string.h>

#include "types.h"
#include "tables.h"

void print_block(const u8 *a, u64 length_bytes);

void stribog(const u8 *message, u16 hash_size, u8 *out);

#endif