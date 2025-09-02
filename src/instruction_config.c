#include "../include/chip8.h"

Bit_mask_t BIT_MAP[0x10] = {

   { .INDEX = 0x0, .OFFSET = 0x0FFF },
   { .INDEX = 0x1, .OFFSET = 0 },
   { .INDEX = 0x2, .OFFSET = 0 },
   { .INDEX = 0x3, .OFFSET = 0 },
   { .INDEX = 0x4, .OFFSET = 0 },

   { .INDEX = 0x5, .OFFSET = 0 },
   { .INDEX = 0x6, .OFFSET = 0 },
   { .INDEX = 0x7, .OFFSET = 0 },
   { .INDEX = 0x8, .OFFSET = 0xf },
   { .INDEX = 0x9, .OFFSET = 0 },

   { .INDEX = 0xA, .OFFSET = 0 },
   { .INDEX = 0xB, .OFFSET = 0 },
   { .INDEX = 0xC, .OFFSET = 0 },
   { .INDEX = 0xD, .OFFSET = 0 },
   { .INDEX = 0xE, .OFFSET = 0xFF },
   { .INDEX = 0xF, .OFFSET = 0xFF }
};

void Instruction_decode(Config_Instruction_t* instruction, const uint16_t opcode)
{
    instruction->OPCODE = opcode;
    instruction->ID     = (opcode & 0xF000) >> 12;
    instruction->NNN    = opcode & 0xFFF;
    instruction->NN     = opcode & 0xFF;
    instruction->N      = opcode & 0xF;
    instruction->X      = (opcode & 0xF00) >> 8; 
    instruction->Y      = (opcode & 0xF0) >> 4;
}

