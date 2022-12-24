#ifndef FUNCT3_H
#define FUNCT3_H

#include <iostream>

// ensure that the underlying datatype for enum is 
// 32 bit integer value
enum Funct3 : int32_t {
    ADD = 0b000,
    SUB = 0b000,
    ADDI = 0b000,
    SLLI = 0b001,
    SLT = 0b010,
    SLTI = 0b010,
    SLTIU = 0b011,
    SLTU = 0b011,

    
    XOR  = 0b100,
    XORI = 0b100,
    SRL  = 0b101,
    SRLI = 0b101,
    SRA  = 0b101,
    SRAI = 0b101,


    OR   = 0b110,
    ORI  = 0b110,
    AND  = 0b111,
    ANDI = 0b111,

    BEQ  = 0b000,
    BNE  = 0b001,
    BLT  = 0b100,
    BGE  = 0b101,
    BLTU = 0b110,
    BGEU = 0b111,

    LB  = 0b000,
    SB  = 0b000,
    LH  = 0b001,
    SH  = 0b001,
    LW  = 0b010,
    SW  = 0b010,
    LBU = 0b100,
    LHU = 0b101,

    //? ??
    ECALL  = 0b000,
    CSRRW  = 0b001,
    CSRRS  = 0b010,
    CSRRC  = 0b011,
    CSRRWI = 0b101,
    CSRRSI = 0b110,
    CSRRCI = 0b111,
};


#endif
