#include "../include/chip8.h"

void(*INSTRUCTION_SET[INSTRUCTION_SET_SIZE])(Chip8_t*);

void chip8_load_instruction()
{

    memset(INSTRUCTION_SET, 0, sizeof(INSTRUCTION_SET));

    INSTRUCTION_SET[0x0000] = Instruction_0NNN_SYS;
    INSTRUCTION_SET[0x00E0] = Instruction_00E0_CLS;
    INSTRUCTION_SET[0x00EE] = Instruction_00EE_RET;
    
    INSTRUCTION_SET[0x1000] = Instruction_1NNN_JP;
    INSTRUCTION_SET[0x2000] = Instruction_2NNN_CALL;
    INSTRUCTION_SET[0x3000] = Instruction_3XNN_SE;

    INSTRUCTION_SET[0x4000] = Instruction_4XNN_SNE;
    INSTRUCTION_SET[0x5000] = Instruction_5XY0_SE;
    INSTRUCTION_SET[0x6000] = Instruction_6XNN_LD;
    INSTRUCTION_SET[0x7000] = Instruction_7XNN_ADD;
    
    INSTRUCTION_SET[0x8000] = Instruction_8XY0_LD;
    INSTRUCTION_SET[0x8001] = Instruction_8XY1_OR;
    INSTRUCTION_SET[0x8002] = Instruction_8XY2_AND;
    INSTRUCTION_SET[0x8003] = Instruction_8XY3_XOR;
    INSTRUCTION_SET[0x8004] = Instruction_8XY4_ADD;
    INSTRUCTION_SET[0x8005] = Instruction_8XY5_SUB;
    INSTRUCTION_SET[0x8006] = Instruction_8XY6_SHR;
    INSTRUCTION_SET[0x8007] = Instruction_8XY7_SUBN;
    INSTRUCTION_SET[0x800E] = Instruction_8XYE_SHL;

    INSTRUCTION_SET[0x9000] = Instruction_9XY0_SNE;
    INSTRUCTION_SET[0xA000] = Instruction_ANNN_LD;
    INSTRUCTION_SET[0xB000] = Instruction_BNNN_AND;
    INSTRUCTION_SET[0xC000] = Instruction_CXNN_AND;
    INSTRUCTION_SET[0xD000] = Instruction_DXYN_DRW;

    INSTRUCTION_SET[0xE09E] = Instruction_EX9E_SKP;
    INSTRUCTION_SET[0xE0A1] = Instruction_EXA1_SKNP;

    INSTRUCTION_SET[0xF007] = Instruction_FX07_LD;
    INSTRUCTION_SET[0xF00A] = Instruction_FX0A_LD;
    INSTRUCTION_SET[0xF015] = Instruction_FX15_LD;
    INSTRUCTION_SET[0xF018] = Instruction_FX18_LD;
    INSTRUCTION_SET[0xF01E] = Instruction_FX1E_ADD;
    INSTRUCTION_SET[0xF029] = Instruction_FX29_LD;
    INSTRUCTION_SET[0xF033] = Instruction_FX33_LD;
    INSTRUCTION_SET[0xF055] = Instruction_FX55_LD;
    INSTRUCTION_SET[0xF065] = Instruction_FX65_LD;

}

void chip8_init(Chip8_t* c)
{
    srand(time(NULL));

    memset(c->RAM, 0, MEMORY_SIZE);
    memset(c->V, 0, MAX_VALUE);
    memset(c->KEYBOARD, 0, MAX_VALUE);
    memset(c->STACK, 0, sizeof(c->STACK));
    memset(c->PIXELS, 0, RESOLUTION);

    c->I           = 0;
    c->SOUND_TIMER = 0;
    c->DELAY_TIMER = 0;
    c->PC          = START_PROGRAM;
    c->SP          = 0;
    c->KEY_PRESSED = 0;
    c->EXTENTION   = CHIP8;
    
    const uint8_t fonts[5 * 16] = {
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
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    
    memcpy(c->RAM, fonts, 5 * 16);

    Chip8_load_instruction();

}

void chip8_load_rom(Chip8_t* c, const char* filepath)
{
    FILE* file = fopen(filepath, "rb");

    if(file == NULL)
    {
        fprintf(stderr, "Failed to load ROM : %s.\n", filepath);
        return;
    }


    long byte_count = 0;
    fseek(file, 0L, SEEK_END);
    byte_count      = ftell(file);
    fseek(file, 0L, SEEK_SET);

    printf("Size of the ROM : %ld.\n", byte_count);

    long byte_read = fread(c->RAM + c->PC, sizeof(uint8_t), byte_count, file);
    
    if(byte_count != byte_read)
        fprintf(stderr, "Failed to read ROM : %s.\n", filepath);

    fclose(file);

}

void chip8_fetch(Chip8_t* c)
{
    const uint16_t opcode = c->RAM[c->PC] << 8 | c->RAM[c->PC + 1];

    Instruction_decode(&c->CURRENT_INSTRUCTION, opcode);

    c->PC += 2;
}

void chip8_decode(Chip8_t* c, uint16_t* index)
{
    uint16_t INDEX = 0;
    INDEX = BIT_MAP[_ID].INDEX << 12 |(_NNN & BIT_MAP[_ID].OFFSET);

    if( _ID == 0 && _NNN != 0xE0 && _NNN != 0xEE )
        INDEX = 0;

    *index = INDEX;

}

void chip8_process_instruction(Chip8_t* c)
{
    Chip8_fetch(c);
    uint16_t index = _ID;

    Chip8_decode(c, &index);

    
    INSTRUCTION_SET[index](c);
}