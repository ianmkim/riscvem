#ifndef OPS_H
#define OPS_H

#include <iostream>

// ensure that the underlying datatype for enum is 
// 32 bit integer value
enum Ops : int32_t {
    LUI    = 0b0110111, //load upper immidiate
    LOAD   = 0b0000011,
    STORE  = 0b0100011,

    AUIPC  = 0b0010111, // add upper immediate to pc
    BRANCH = 0b1100011,
    JAL    = 0b1101111,
    JALR   = 0b1100111,

    IMM    = 0b0010011,
    OP     = 0b0110011,

    MISC   = 0b0001111,
    SYSTEM = 0b1110011,
};


#endif
