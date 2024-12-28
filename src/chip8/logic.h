#pragma once
#include "context.h"

void op_00E0(ChipContext *ctx);
void op_1NNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3);
void op_6XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2);
void op_7XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2);
void op_ANNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3);
void op_DXYN(ChipContext *ctx, u8 x, u8 y, u8 n);

void chip_cycle(ChipContext *ctx);
void decode(u16 opcode, ChipContext *ctx);