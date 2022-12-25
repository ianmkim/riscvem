#include "cpu.hpp"

#include "memory.hpp"
#include "registers.hpp"
#include "utils.hpp"

#include "ops.hpp"
#include "funct3.hpp"


bool isAltMode(int32_t funct7, Funct3 funct3, Ops opcode){
    return (funct7 == 0b0100000) 
        && (opcode == Ops::OP || 
            (opcode == Ops::IMM && funct3 == Funct3::SRAI));
}

bool needRegWriteback(Ops opcode){
    return (
        opcode == Ops::JAL   ||
        opcode == Ops::JALR  ||
        opcode == Ops::AUIPC ||
        opcode == Ops::LUI   ||
        opcode == Ops::OP    ||
        opcode == Ops::IMM   ||
        opcode == Ops::LOAD
    );
}


bool step(Memory &mem, Regfile &reg){
    int32_t ins = mem.readSegment(reg.regs[PC]);
    Ops opcode = (Ops)get_bits(ins, 6, 0);

    // a lot of this parsing is redundant, depending on the instruction
    // type, we will not need certain variables. However, overhead is small
    // and makes the actual computation much easier.

    // load funct3
    Funct3 funct3 = (Funct3)get_bits(ins, 14, 12);

    // load funct7
    int32_t funct7 = get_bits(ins, 31, 25);

    // load the immediate values
    int32_t imm_i = sign_extend(get_bits(ins, 31, 20), 12); 
    int32_t imm_s = sign_extend((get_bits(ins, 31, 25) << 5)  | 
                    get_bits(ins, 11, 7), 12);
    int32_t imm_b = sign_extend((get_bits(ins, 32, 31) << 12) | 
                        (get_bits(ins, 30, 25) << 5)  |
                        (get_bits(ins, 11, 8)  << 1)  |
                        (get_bits(ins, 8, 7)   << 11), 13);
    int32_t imm_u = sign_extend(get_bits(ins, 31, 12) << 12, 32);
    int32_t imm_j = sign_extend((get_bits(ins, 32, 31) << 20) |
                        (get_bits(ins, 30, 21) << 1)  |
                        (get_bits(ins, 21, 20) << 11) |
                        (get_bits(ins, 19, 12) << 12), 21);

    int32_t imm;
    switch(opcode){
        case ops::STORE:
            imm = imm_s;
            break;

        case Ops::JAL:
            imm = imm_j;
            break;

        case ops::BRANCH:
            imm = imm_b;
            break;

        case ops::OP:
            imm = vs2;
            break;

        case ops::AUIPC:
        case ops::LUI:
            imm = imm_u;
            break;

        case ops::IMM:
        case ops::LOAD:
        case ops::JALR:
        case ops::SYSTEM:
        case ops::MISC:
            imm = imm_i;
            break;
            
        default:
            throw std::runtime_error("Unknown OP code");
    }

    


    int32_t rd = get_bits(ins, 11, 7);

    int32_t vs1 = reg.regs[get_bits(ins, 19, 15)];
    int32_t vs2 = reg.regs[get_bits(ins, 24, 20)];
    int32_t vpc = reg.regs[PC];

    int32_t arith_left = (opcode == Ops::JAL || opcode == Ops::BRANCH || opcode == Ops.AUIPC) ? vpc : (opcode == Ops::LUI ? 0 : vs1);
    Funct3 = (opcode == Ops::OP || opcode == Ops::IMM) ? funct3 : Funct3::ADD;
    
    
}
