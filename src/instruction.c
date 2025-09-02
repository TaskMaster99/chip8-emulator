#include "../include/chip8.h"

void debbug_instruction(Chip8_t* c, const uint8_t condition_test, const char* instruction_name,const char* succes_message, const char* failure_message)
{
    if(condition_test)
        fprintf(stderr, BLUE "(%s)" RESET YELLOW "Instruction %04X" RESET GREEN " %s.\n" RESET, instruction_name, c->CURRENT_INSTRUCTION.OPCODE, succes_message);
    else
        fprintf(stderr, BLUE "(%s)" RESET YELLOW "Instruction %04X" RESET RED " %s.\n" RESET, instruction_name, c->CURRENT_INSTRUCTION.OPCODE, failure_message);
}

void Instruction_0NNN_SYS(Chip8_t* c)
{
    c->PC = _NNN;
    //debbug_instruction(c, 1, "SYS","deprecated", NULL);

}

void Instruction_00E0_CLS(Chip8_t* c)
{
    memset(c->PIXELS, 0, RESOLUTION);
    //uint8_t zero_buffer[RESOLUTION] = {0};
   // uint8_t succes_condition  = memcmp(zero_buffer, c->PIXELS, RESOLUTION) == 0;

    //debbug_instruction(c, succes_condition, "CLEAR","succesfully cleared the screen (setting the pixel state at 0)",
    //                                        "failded to clear the screen (setting the pixel state at 0)");

}


void Instruction_00EE_RET(Chip8_t* c)
{

    c->PC = c->STACK[--c->SP];
    uint8_t succes_condition = c->PC == c->STACK[c->SP];

    char succes_buffer[100];
    char failure_buffer[100];

    //sprintf(succes_buffer, "succesfully return subroutine, i.e PC = %04X and STACK[STACK POINTER] = %02X", c->PC, c->STACK[c->SP]);
    //sprintf(failure_buffer, "failed to return subroutine, i.e PC = %04X and STACK[STACK POINTER] = %02X", c->PC, c->STACK[c->SP]);

    //debbug_instruction(c, succes_condition, "RET",succes_buffer, failure_buffer);

}

void Instruction_1NNN_JP(Chip8_t* c)
{
    c->PC = _NNN;
    //uint8_t succes_condition = c->PC == _NNN;

    //char succes_buffer[100];
    //char failure_buffer[100];

    //sprintf(succes_buffer, "succesfully jumped to %04X, i.e PC = %04X ", _NNN, c->PC);
    //sprintf(failure_buffer, "failed to jump to %04X, i.e PC = %04X ",  _NNN, c->PC);

    //debbug_instruction(c, succes_condition, "JUMP",succes_buffer, failure_buffer);
}

void Instruction_2NNN_CALL(Chip8_t* c)
{
    c->STACK[c->SP++] = c->PC;
    uint16_t previous_pc = c->PC;
    
    c->PC = _NNN;
    uint8_t succes_condition = c->STACK[c->SP - 1] == previous_pc;

    //char succes_buffer[100];
    //char failure_buffer[100];

    //sprintf(succes_buffer, "succesfully call %04X, i.e top stack = %04X pc = %04x ", c->STACK[c->SP], previous_pc, previous_pc);
    //sprintf(failure_buffer, "failed to call %04X, i.e top stack = %04X pc = %04x ",  c->STACK[c->SP], previous_pc, previous_pc);

    //debbug_instruction(c, succes_condition, "CALL",succes_buffer, failure_buffer);

}

void Instruction_3XNN_SE(Chip8_t* c)
{
    c->PC = c->V[_X] == _NN ? c->PC + 2 : c->PC;
    //debbug_instruction(c, 1, "SE"," ", NULL);

}

void Instruction_4XNN_SNE(Chip8_t* c)
{
    c->PC = c->V[_X] != _NN ? c->PC + 2 : c->PC;
    //debbug_instruction(c, 1, "SNE"," ", NULL);

}


void Instruction_5XY0_SE(Chip8_t* c)
{
    c->PC = c->V[_X] == c->V[_Y] ? c->PC + 2 : c->PC;
    debbug_instruction(c, 1, "SE"," ", NULL);

}

void Instruction_6XNN_LD(Chip8_t* c)
{
    c->V[_X] = _NN;
    
    // char succes_buffer[100];
    // char failure_buffer[100];

    // uint8_t succes_condition  =  c->V[_X] == _NN;

    // sprintf(succes_buffer, "succesfully load value NN to register VX, i.e X = %01X, V%01X = %02X and NN = %02X", _X,_X,
    //                                                                                                     c->V[_X], 
    //                                                                                                     _NN);
    // sprintf(failure_buffer, "failed to load value NN to register VX, i.e X = %01X, V%01X = %02X and NN = %02X", _X,_X,
    //                                                                                                     c->V[_X], 
                                                                                                        // _NN);

    // debbug_instruction(c, succes_condition, "LOAD",succes_buffer, failure_buffer);


}

void Instruction_7XNN_ADD(Chip8_t* c)
{
	c->V[_X] += _NN;
    // debbug_instruction(c, 1, "ADD"," ", NULL);

}

void Instruction_8XY0_LD(Chip8_t* c)
{
    c->V[_X] = c->V[_Y];
    debbug_instruction(c, 1, "LOAD"," ", NULL);

}

void Instruction_8XY1_OR(Chip8_t* c)
{

    c->V[_X] |= c->V[_Y];
    c->V[0xF] = c->EXTENTION == CHIP8 ? 0x0 : c->V[0xF];
    // debbug_instruction(c, 1, "OR"," ", NULL);

}

void Instruction_8XY2_AND(Chip8_t* c)
{
    c->V[_X] &= c->V[_Y];
    c->V[0xF] = c->EXTENTION == CHIP8 ? 0x0 : c->V[0xF];
    // debbug_instruction(c, 1, "AND"," ", NULL);

}


void Instruction_8XY3_XOR(Chip8_t* c)
{
    c->V[_X] ^= c->V[_Y];
    c->V[0xF] = c->EXTENTION == CHIP8 ? 0x0 : c->V[0xF];
    // debbug_instruction(c, 1, "XOR"," ", NULL);

}

void Instruction_8XY4_ADD(Chip8_t* c)
{
    uint16_t result = c->V[_X] + c->V[_Y];
    uint8_t carry = result > 0xFF ? 1 : 0;
    c->V[_X] = (result & 0xFF);
    c->V[0xF] = carry;     
    // debbug_instruction(c, 1, "ADD"," ", NULL);

}

void Instruction_8XY5_SUB(Chip8_t* c)
{
    uint8_t carry = c->V[_X] >= c->V[_Y] ? 1 : 0;
    
    c->V[_X] -= c->V[_Y];
    c->V[0xF] = carry;
    // debbug_instruction(c, 1, "SUB"," ", NULL);

}

void Instruction_8XY6_SHR(Chip8_t* c)
{
    if(c->EXTENTION == CHIP8)
    {
        uint8_t carry = c->V[_Y] & 0x1;
        c->V[_X] = c->V[_Y] >> 1;
        c->V[0xF] = carry;
    }
    else
    {
        uint8_t carry = c->V[_X] & 0x1;
        c->V[_X] >>= 1;
        c->V[0xF] = carry;
    }

    // debbug_instruction(c, 1, "SHR"," ", NULL);

}

void Instruction_8XY7_SUBN(Chip8_t* c)
{
    uint8_t carry = c->V[_Y] >= c->V[_X] ? 1 : 0;

    c->V[_X] = c->V[_Y] - c->V[_X];

    c->V[0xF] = carry;

    // debbug_instruction(c, 1, "SUBN"," ", NULL);

}

void Instruction_8XYE_SHL(Chip8_t* c)
{
    

    if(c->EXTENTION == CHIP8)
    {
        uint8_t carry = (c->V[_Y] & 0x80) >> 7;
        c->V[_X] = c->V[_Y] << 1;
        c->V[0xF] = carry;
    }
    else
    {
        uint8_t carry = (c->V[_X] & 0x80) >> 7;
        c->V[_X] <<= 1;
        c->V[0xF] = carry;
    }
    // debbug_instruction(c, 1, "SHL"," ", NULL);

}


void Instruction_9XY0_SNE(Chip8_t* c)
{
    c->PC = c->V[_X] != c->V[_Y] ? c->PC + 2 : c->PC;
    // debbug_instruction(c, 1, "SNE"," ", NULL);

}

void Instruction_ANNN_LD(Chip8_t* c)
{
    c->I = _NNN;

    // char succes_buffer[100];
    // char failure_buffer[100];

    // uint8_t succes_condition  =  c->I == _NNN;

    // sprintf(succes_buffer, "succesfully load value NNN to register I, i.e I = %04X and NNN = %03X", c->I,_NNN);
    // sprintf(failure_buffer, "failed to load value NNN to register I, i.e I = %04X and NNN = %03X", c->I,_NNN);

    // debbug_instruction(c, succes_condition, "LOAD",succes_buffer, failure_buffer);

}

void Instruction_BNNN_AND(Chip8_t* c)
{
    c->PC = _NNN + c->V[0];
    // debbug_instruction(c, 1, "AND"," ", NULL);

}

void Instruction_CXNN_AND(Chip8_t* c)
{
    c->V[_X] = (rand() % 256) & _NN;
    // debbug_instruction(c, 1, "AND"," ", NULL);

}

void Instruction_DXYN_DRW(Chip8_t* c)
{
    uint16_t xPos = c->V[_X] % 64;
    uint16_t yPos = c->V[_Y] % 32;

    c->V[0xF] = 0;

    for(size_t i = 0; i < _N; ++i)
    {
        uint8_t byte = c->RAM[c->I + i];
        xPos = c->V[_X] % 64;

        for(int8_t j = 7; j >= 0; --j)
        {
            if(((byte >> j) & 0x1) && c->PIXELS[ yPos * 64 + xPos])
                c->V[0xF] = 1;

            c->PIXELS[ yPos * 64 + xPos] ^= ((byte >> j) & 0x1);
            
            if(++xPos >= 64)break;
        }
        if(++yPos >= 32)break;
    }
    // debbug_instruction(c, 1, "DRAW","draw on the screen", NULL);
}

void Instruction_EX9E_SKP(Chip8_t* c)
{
    c->PC = c->KEYBOARD[c->V[_X]] ? c->PC + 2 : c->PC;
    // debbug_instruction(c, 1, "SKP"," ", NULL);

}


void Instruction_EXA1_SKNP(Chip8_t* c)
{
    c->PC = !c->KEYBOARD[c->V[_X]] ? c->PC + 2 : c->PC;
    // debbug_instruction(c, 1, "SKNP"," ", NULL);

}

void Instruction_FX07_LD(Chip8_t* c)
{
    c->V[_X] = c->DELAY_TIMER;
    // debbug_instruction(c, 1, "LOAD"," ", NULL);

}

void Instruction_FX0A_LD(Chip8_t* c)
{
    static uint8_t KEY_PRESSED = 0;
    static uint8_t key_p = 0xFF;
    for(uint8_t i = 0; i < MAX_VALUE; ++i)
    {
        if(c->KEYBOARD[i])
        {
            key_p = i;
            KEY_PRESSED = 1;
            break;
        }
    }
    if(!KEY_PRESSED)
        c->PC -= 2;
    else{
        if(c->KEYBOARD[key_p])
            c->PC -= 2;
        else
        {
            c->V[_X] = key_p;
            KEY_PRESSED = 0;
        }
    }
    // debbug_instruction(c, 1, "LOAD"," ", NULL);

}

void Instruction_FX15_LD(Chip8_t* c)
{
    c->DELAY_TIMER = c->V[_X];
    // debbug_instruction(c, 1, "LOAD"," ", NULL);

}

void Instruction_FX18_LD(Chip8_t* c)
{
    c->SOUND_TIMER = c->V[_X];
    // debbug_instruction(c, 1, "LOAD"," ", NULL);

}

void Instruction_FX1E_ADD(Chip8_t* c)
{
    c->I += c->V[_X];
    // debbug_instruction(c, 1, "ADD"," ", NULL);

}


void Instruction_FX29_LD(Chip8_t* c)
{
    c->I = c->V[_X] * 5;
    // debbug_instruction(c, 1, "LOAD"," ", NULL);

}

void Instruction_FX33_LD(Chip8_t* c)
{
    c->RAM[c->I    ] = c->V[_X] / 100;
    c->RAM[c->I + 1] = (c->V[_X] / 10) % 10;
    c->RAM[c->I + 2] = c->V[_X] % 10;
    // debbug_instruction(c, 1, "LOAD"," ", NULL);

}

void Instruction_FX55_LD(Chip8_t* c)
{
    if(c->EXTENTION == CHIP8)
    {
        for(uint8_t i = 0; i <= _X; ++i)
        {    
            c->RAM[c->I] = c->V[i];
            ++c->I;
        }
    }
    else
        memcpy(c->RAM + c->I, c->V, _X + 1);
    // debbug_instruction(c, 1, "LOAD"," ", NULL);

}

void Instruction_FX65_LD(Chip8_t* c)
{
    if(c->EXTENTION == CHIP8)
    {
        for(uint8_t i = 0; i <= _X; ++i)
        {
            c->V[i] = c->RAM[c->I];
            ++c->I;
        }
    }
    else
        memcpy(c->V, c->RAM + c->I, _X + 1);
    // debbug_instruction(c, 1, "LOAD"," ", NULL);

}
