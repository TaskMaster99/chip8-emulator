#ifndef INSTRUCTION_CONFIG_H
#define INSTRUCTION_CONFIG_H

#include <stdint.h> 

typedef struct Bit_mask
{

   uint8_t ID;
   uint16_t REMAINDER;

}Bit_mask_t;

typedef struct Config_Instruction
{

   uint16_t OPCODE; // 16-bit  Instruction  opcode
   uint16_t NNN;    // lowest  12-bit of opcode
   uint16_t NN;     // lowest  8-bits of opcode
   uint8_t  N;      // lowest  4-bits of opcode
   uint8_t  ID;
   uint8_t  X;      // lowest  4-bits of the high bits of opcode
   uint8_t  Y;      // highest 4-bits of the low bits of opcode

}Config_Instruction_t;

extern Bit_mask_t BIT_MAP[0x10];

void Instruction_decode(Config_Instruction_t* instruction, const uint16_t opcode);
#endif