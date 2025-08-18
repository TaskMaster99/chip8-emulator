#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

typedef char                    char8_t;
typedef float                   float32_t;
typedef double                  float64_t;

#define MAX_SIZE                4096
#define DEBUT_PROGRAM           0x200
#define DEBUT_ETI_660_PROGRAM   0X600
#define MAX_STACK               16
#define MAX_REGISTER               16
#define MAX_PIXELS              64*32
#define HIGH_BIT_SHIFT          >> 12
#define MAX_INSTRCUTION_SET     16
#define MAX_FONT                16 * 5
#define MAX_KEYPAD              16

typedef struct Instruction {
    uint8_t  opcode_id;
    uint8_t  X ;
    uint8_t  Y ;
    uint16_t NNN ;
    uint8_t  NN;
    uint8_t  N ;

}Instruction_t;

#define DISPLAY_INSTRUCTION(INS){\
    printf("-------------\n");\
\
    printf("----ID : 0X%01X\n", INS.opcode_id);\
    printf("----NNN : 0X%03X\n", INS.NNN);\
    printf("----NN : 0X%02X\n", INS.NN);\
\
    printf("----N : 0X%01X\n", INS.N);\
    printf("----X : 0X%01X\n", INS.X);\
    printf("----Y : 0X%01X\n", INS.Y);\
}\

typedef struct Chip_8 {
    uint8_t   MEMORY[MAX_SIZE];         // 4KB RAM
    uint16_t  STACK[MAX_STACK];         // STACK 16 x 16-bit
    uint8_t   REGISTER_V[MAX_REGISTER]; // 16 bytes register
    uint8_t   FRAME_BUFFER[MAX_PIXELS]; // FRAME BUFFER
    uint8_t   KEYPAD[MAX_KEYPAD];       // KEYCODE
    uint16_t  REGISTER_PC;              // 16-bit REGISTER PROGRAM COUNTER
    uint16_t  REGISTER_I;               // 16-bit REGISTER I
    uint8_t   REGISTER_DELAY;           // 8-bit REGISTER DELAY TIMER
    uint8_t   REGISTER_SOUND_TIMERS;    // 8-bit REGISTER SOUND TIMER
    uint8_t   REGISTER_SP;              // 8-bit REGISTER STACK POINTER
}Chip_8_t;

void _chip_init_(Chip_8_t* cpu)
{
    srand(time(NULL));

    memset(cpu->MEMORY, 0, MAX_SIZE * sizeof(uint16_t));
    memset(cpu->STACK, 0, MAX_STACK * sizeof(uint16_t));
    memset(cpu->REGISTER_V, 0, MAX_REGISTER * sizeof(uint8_t));
    memset(cpu->FRAME_BUFFER, 0, MAX_PIXELS * sizeof(uint8_t));
    memset(cpu->KEYPAD, 0, MAX_KEYPAD * sizeof(uint8_t));

    cpu->REGISTER_I            = 0;
    cpu->REGISTER_DELAY        = 0;
    cpu->REGISTER_SOUND_TIMERS = 0;
    cpu->REGISTER_PC           = DEBUT_PROGRAM; // START OF ANY CODE
    cpu->REGISTER_SP           = 0;

    const uint8_t  FONT[MAX_FONT]  =  {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    };

    memcpy(cpu->MEMORY, FONT, MAX_FONT * sizeof(uint8_t));
}

void _chip_decode(const uint16_t opcode, Instruction_t* nibles)
{
    (*nibles) = (Instruction_t){
        .opcode_id = (opcode & 0XF000) >> 12,
        .X         = (opcode & 0X0F00) >> 8,
        .Y         = (opcode & 0X00F0) >> 4,
        .NNN       = (opcode & 0X0FFF),
        .NN        = (opcode & 0X00FF) ,
        .N         = (opcode & 0X000F)
    };
}

void _chip_process_ins(Chip_8_t* cpu, const uint16_t opcode)
{
    Instruction_t n;

    _chip_decode(opcode, &n);

    static uint16_t result ;
    static uint8_t  random_number;
    static uint8_t  result8_t;
    
    switch (n.opcode_id)
    {
    case 0x0:
        
        switch (n.NNN)
        {
        case 0x0E0: // CLS
            memset(cpu->FRAME_BUFFER, 0, MAX_PIXELS * sizeof(uint8_t));
            break;

        case 0x0EE: // RET
            cpu->REGISTER_PC  = cpu->STACK[cpu->REGISTER_SP];
            cpu->REGISTER_SP -= 1;
            break;
        
        default:
            break;
        }
        
        break;
    
    case 0x1: // JP NNN
        cpu->REGISTER_PC = n.NNN;
        break;

    case 0x2: // CALL NNN
        cpu->REGISTER_SP            += 1;
        cpu->STACK[cpu->REGISTER_SP] = cpu->REGISTER_PC;
        cpu->REGISTER_PC             = n.NNN;
        break;

    case 0x3: // SE VX, byte
        cpu->REGISTER_PC = cpu->REGISTER_V[n.X] == n.NN ? cpu->REGISTER_PC + 2 : cpu->REGISTER_PC;
        break;
    
    case 0x4: // SNE VX, byte
        cpu->REGISTER_PC = cpu->REGISTER_V[n.X] != n.NN ? cpu->REGISTER_PC + 2 : cpu->REGISTER_PC;
        break;

    case 0x5: // SE VX, VY
        if(n.N == 0)
            cpu->REGISTER_PC = cpu->REGISTER_V[n.X] != cpu->REGISTER_V[n.Y] ? cpu->REGISTER_PC + 2 : cpu->REGISTER_PC;
        break;

    case 0x6:
        cpu->REGISTER_V[n.X] = n.NN;
        break;
    
    case 0x7:
        cpu->REGISTER_V[n.X] += n.NN;
        break;
    
    case 0x8:
        switch (n.N)
        {
        case 0x0:
            cpu->REGISTER_V[n.X] = cpu->REGISTER_V[n.Y];
            break;
        case 0x1:
        
            cpu->REGISTER_V[n.X] |= cpu->REGISTER_V[n.Y];
            break;

        case 0x2:
            cpu->REGISTER_V[n.X] &= cpu->REGISTER_V[n.Y];
            break;

        case 0x3:
            cpu->REGISTER_V[n.X] ^= cpu->REGISTER_V[n.Y];
            break;

        case 0x4:
            result               = cpu->REGISTER_V[n.X] + cpu->REGISTER_V[n.Y];
            cpu->REGISTER_V[0xF] = result > 0xFF ? 1 : 0;
            cpu->REGISTER_V[n.X] = result & 0x0F;
            break;

        case 0x5:
            cpu->REGISTER_V[0xF] = cpu->REGISTER_V[n.X] > cpu->REGISTER_V[n.Y] ? 1 : 0;
            result               = cpu->REGISTER_V[n.X] - cpu->REGISTER_V[n.Y];
            cpu->REGISTER_V[n.X] = result & 0x0F;
            break;

        case 0x6:
            cpu->REGISTER_V[0xF] = cpu->REGISTER_V[n.X] & 0x01;
            result               = cpu->REGISTER_V[n.X] / 2;
            cpu->REGISTER_V[n.X] = result & 0x0F;
            break;

        case 0x7:
            cpu->REGISTER_V[0xF] = cpu->REGISTER_V[n.X] < cpu->REGISTER_V[n.Y];
            result               = cpu->REGISTER_V[n.Y] - cpu->REGISTER_V[n.X];
            cpu->REGISTER_V[n.X] = result & 0x0F;
            break;

        case 0xE:
            cpu->REGISTER_V[0xF] = (cpu->REGISTER_V[n.X] & 0x80) >> 7;
            result               = cpu->REGISTER_V[n.X] / 2;
            cpu->REGISTER_V[n.X] = result & 0x0F;            
            break;

        default:
            break;
        }

        break;

    case 0x9:
        if(n.N == 0)
            cpu->REGISTER_PC = cpu->REGISTER_V[n.X] != cpu->REGISTER_V[n.Y] ? cpu->REGISTER_PC + 2 : cpu->REGISTER_PC;
        break;

    case 0xA:
        cpu->REGISTER_I     = n.NNN;
        break;

    case 0xB:
        cpu->REGISTER_PC    = n.NNN + cpu->REGISTER_V[0x0];
        break;

    case 0xC:
        random_number        = (uint8_t)rand() % 256;
        result8_t            = random_number & n.NN;
        cpu->REGISTER_V[n.X] = result8_t;
        break;

    case 0xD:
        //TODO
        break;

    case 0xE:
        switch (n.NN)
        {
        case 0x9E:
            //TODO
            break;

        case 0xA1:
            //TODO
            break;

        default:
            break;
        }
        
        break;

    case 0xF:
        switch (n.NN)
        {
        case 0x07:
            cpu->REGISTER_V[n.X] = cpu->REGISTER_DELAY;
            break;

        case 0x0A:
            //TODO
            break;

        case 0x15:
            cpu->REGISTER_DELAY = cpu->REGISTER_V[n.X];
            break;

        case 0x18:
            cpu->REGISTER_SOUND_TIMERS = cpu->REGISTER_V[n.X];
            break;

        case 0x1E:
            cpu->REGISTER_I += cpu->REGISTER_V[n.X]; 
            break;

        case 0x29:
            cpu->REGISTER_I = cpu->MEMORY[cpu->REGISTER_V[n.X] * 5];
            break;

        case 0x33:
            cpu->MEMORY[cpu->REGISTER_I]     = (cpu->REGISTER_V[n.X] & 0x04) >> 2;
            cpu->MEMORY[cpu->REGISTER_I + 1] = (cpu->REGISTER_V[n.X] & 0x02) >> 1;
            cpu->MEMORY[cpu->REGISTER_I + 2] = (cpu->REGISTER_V[n.X] & 0x01);
            break;

        case 0x55: // LD [I], VX
            memcpy(cpu->MEMORY + cpu->REGISTER_I, cpu->REGISTER_V, (n.X +1)* sizeof(uint8_t));
            break;

        case 0x65: // LD VX, [1]
            memcpy(cpu->REGISTER_V, cpu->MEMORY + cpu->REGISTER_I, (n.X +1) * sizeof(uint8_t));
            break;

        default:
            
            break;
        }
        break;

    default:
        break;
    }
}


#endif