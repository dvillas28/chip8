#pragma once
#include <stdlib.h>
#include <stdbool.h>
#include "context.h"

void op_00E0(ChipContext *ctx);                      // clear screen
void op_1NNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3); // jump
void op_00EE(ChipContext *ctx);                      // return
void op_2NNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3); // call
void op_3XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2);  // skip if equal
void op_4XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2);  // skip if not equal
void op_5XY0(ChipContext *ctx, u8 x, u8 y);          // skip if Vx == Vy
void op_9XY0(ChipContext *ctx, u8 x, u8 y);          // skip if Vx != Vy
void op_6XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2);  // set register vx to the value nn
void op_7XNN(ChipContext *ctx, u8 x, u8 n1, u8 n2);  // add value nn to vx
void op_8XY0(ChipContext *ctx, u8 x, u8 y);          // Vx <- Vy
void op_8XY1(ChipContext *ctx, u8 x, u8 y);          // Vx <- Vx OR Vy
void op_8XY2(ChipContext *ctx, u8 x, u8 y);          // Vx <- Vx AND Vy
void op_8XY3(ChipContext *ctx, u8 x, u8 y);          // Vx <- Vx XOR Vy
void op_8XY4(ChipContext *ctx, u8 x, u8 y);          // Vx <- Vx + Vy, this will affect the carry flag
void op_8XY5(ChipContext *ctx, u8 x, u8 y);          // Vx <- Vx - Vy, this will affect the carry flag
void op_8XY7(ChipContext *ctx, u8 x, u8 y);          // Vx <- Vy - Vx, this will affect the carry flag
void op_8XY6(ChipContext *ctx, u8 x, u8 y);          // Vx <- SHR Vx
void op_8XYE(ChipContext *ctx, u8 x, u8 y);          // Vx <- SHL Vy
void op_ANNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3); // set index
void op_BNNN(ChipContext *ctx, u8 n1, u8 n2, u8 n3); // jump with offset
void op_CXNN(ChipContext *ctx, u8 x, u8 n1, u8 n2);  // Vx <- rand() & NN
void op_DXYN(ChipContext *ctx, u8 x, u8 y, u8 n);    // draw
void op_EX9E(ChipContext *ctx, u8 x);                // skip if key[Vx] is pressed
void op_EXA1(ChipContext *ctx, u8 x);                // skip if key[Vx] is not pressed
void op_FX07(ChipContext *ctx, u8 x);                // Vx <- delay_reg
void op_FX15(ChipContext *ctx, u8 x);                // delay_reg <- Vx
void op_FX18(ChipContext *ctx, u8 x);                // sound_reg <- Vx
void op_FX1E(ChipContext *ctx, u8 x);                // add to index
void op_FX0A(ChipContext *ctx, u8 x);                // get key
void op_FX29(ChipContext *ctx, u8 x);                // font character
void op_FX33(ChipContext *ctx, u8 x);                // binary-coded decimal conversion
void op_FX55(ChipContext *ctx, u8 x);                // store memory
void op_FX65(ChipContext *ctx, u8 x);                // load memory

void chip_cycle(ChipContext *ctx);
void decode(u16 opcode, ChipContext *ctx);