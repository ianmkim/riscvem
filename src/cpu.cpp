#include "cpu.hpp"

#include "memory.hpp"
#include "registers.hpp"
#include "utils.hpp"

#include "ops.hpp"
#include "funct3.hpp"


bool step(Memory &mem, Regfile &reg){
    int ins = mem.readSegment(reg.regs[PC]);
    Ops opcode = (Ops)get_bits(ins, 6, 0);
    Funct3 funct3 = (Funct3)get_bits(ins, 14, 12);
    int funct7 = get_bits(ins, 31, 25);

    int imm_i = sign_extend(get_bits(ins, 31, 20), 12); 
    
    int imm_s = sign_extend((get_bits(ins, 31, 25) << 5)  | get_bits(ins, 11, 7), 12);
    int imm_b = sign_extend((get_bits(ins, 32, 31) << 12) | 
                        (get_bits(ins, 30, 25) << 5)  |
                        (get_bits(ins, 11, 8)  << 1)  |
                        (get_bits(ins, 8, 7)   << 11), 13);
    int imm_u = sign_extend(get_bits(ins, 31, 12) << 12, 32);
    int imm_j = sign_extend((get_bits(ins, 32, 31) << 20) |
                        (get_bits(ins, 30, 21) << 1)  |
                        (get_bits(ins, 21, 20) << 11) |
                        (get_bits(ins, 19, 12) << 12), 21);

    int rd = get_bits(ins, 11, 7);

    int vs1 = reg.regs[get_bits(ins, 19, 15)];
    int vs2 = reg.regs[get_bits(ins, 24, 20)];
    int vpc = reg.regs[PC];
    
    return true;
}
