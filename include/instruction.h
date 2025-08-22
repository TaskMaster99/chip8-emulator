#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "chip8.h"

void Instruction_00E0_CLS(Chip8_t* c);
void Instruction_00EE_RET(Chip8_t* c);
void Instruction_1NNN_JP(Chip8_t* c);
void Instruction_2NNN_CALL(Chip8_t* c);
void Instruction_3XNN_SE(Chip8_t* c);
void Instruction_4XNN_SNE(Chip8_t* c);

void Instruction_5XY0_SE(Chip8_t* c);
void Instruction_6XNN_LD(Chip8_t* c);
void Instruction_7XNN_ADD(Chip8_t* c);
void Instruction_8XY0_LD(Chip8_t* c);
void Instruction_8XY1_OR(Chip8_t* c);
void Instruction_8XY2_AND(Chip8_t* c);

void Instruction_8XY3_XOR(Chip8_t* c);
void Instruction_8XY4_ADD(Chip8_t* c);
void Instruction_8XY5_SUB(Chip8_t* c);
void Instruction_8XY6_SHR(Chip8_t* c);
void Instruction_8XY7_SUBN(Chip8_t* c);
void Instruction_8XYE_SHL(Chip8_t* c);

void Instruction_9XY0_SNE(Chip8_t* c);
void Instruction_ANNN_LD(Chip8_t* c);
void Instruction_BNNN_AND(Chip8_t* c);
void Instruction_CXNN_AND(Chip8_t* c);
void Instruction_DXYN_DRW(Chip8_t* c);
void Instruction_EX9E_SKP(Chip8_t* c);

void Instruction_EXA1_SKNP(Chip8_t* c);
void Instruction_FX07_LD(Chip8_t* c);
void Instruction_FX0A_LD(Chip8_t* c);
void Instruction_FX15_LD(Chip8_t* c);
void Instruction_FX18_LD(Chip8_t* c);
void Instruction_FX1E_ADD(Chip8_t* c);

void Instruction_FX29_LD(Chip8_t* c);
void Instruction_FX33_LD(Chip8_t* c);
void Instruction_FX55_LD(Chip8_t* c);
void Instruction_FX65_LD(Chip8_t* c);


#endif