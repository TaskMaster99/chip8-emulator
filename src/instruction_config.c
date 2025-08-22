#include "../include/instruction_confg.h"

Bit_mask_t BIT_MAP[0x10] = {

   { .ID = 0x0, .REMAINDER = 0x00FF },
   { .ID = 0x1, .REMAINDER = 0x8000 },
   { .ID = 0x2, .REMAINDER = 0x8000 },
   { .ID = 0x3, .REMAINDER = 0x8000 },
   { .ID = 0x4, .REMAINDER = 0x8000 },

   { .ID = 0x5, .REMAINDER = 0x8000 },
   { .ID = 0x6, .REMAINDER = 0x8000 },
   { .ID = 0x7, .REMAINDER = 0x8000 },
   { .ID = 0x8, .REMAINDER = 0x8001 },
   { .ID = 0x9, .REMAINDER = 0x8000 },

   { .ID = 0xA, .REMAINDER = 0x8000 },
   { .ID = 0xB, .REMAINDER = 0x8000 },
   { .ID = 0xC, .REMAINDER = 0x8000 },
   { .ID = 0xD, .REMAINDER = 0x8000 },
   { .ID = 0xE, .REMAINDER = 0x80FF },
   { .ID = 0xF, .REMAINDER = 0x80FF }
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

