#include "cpu.hpp"

#include <stdexcept>

#include "memory.hpp"
#include "registers.hpp"
#include "utils.hpp"

#include "conditionals.hpp"
#include "arithmetics.hpp"

#include "ops.hpp"
#include "funct3.hpp"


bool isAltMode(int32_t funct7, Funct3 funct3, Ops opcode){
    return (funct7 == 0b0100000) 
        && (opcode == Ops::OP || 
            (opcode == Ops::IMM && funct3 == Funct3::SRAI));
}

bool needRegWriteback(Ops opcode){
    return opcode == Ops::JAL   ||
        opcode == Ops::JALR  ||
        opcode == Ops::AUIPC ||
        opcode == Ops::LUI   ||
        opcode == Ops::OP    ||
        opcode == Ops::IMM   ||
        opcode == Ops::LOAD;
}


bool step(Memory &mem, Regfile &reg){
    //mem.dumpAll();
    mem.dump(reg.regs[PC], reg.regs[PC] + 10);
    reg.dump();

    int32_t ins = mem.readSegment(reg.regs[PC]);
    Ops opcode = (Ops)get_bits(ins, 6, 0);
    std::cout << reg.regs[PC] << " " << HEX(ins) << " " << HEX( get_bits(ins, 6, 0) ) << std::endl;

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

    int32_t rd = get_bits(ins, 11, 7);

    int32_t vs1 = reg.regs[get_bits(ins, 19, 15)];
    int32_t vs2 = reg.regs[get_bits(ins, 24, 20)];
    int32_t vpc = reg.regs[PC];

    int32_t arith_left = (opcode == Ops::JAL || opcode == Ops::BRANCH || opcode == Ops::AUIPC) ? vpc : (opcode == Ops::LUI ? 0 : vs1);
    Funct3 arith_funct = (opcode == Ops::OP || opcode == Ops::IMM) ? funct3 : Funct3::ADD;
 
    int32_t imm;
    switch(opcode){
        case Ops::STORE:
            imm = imm_s;
            break;

        case Ops::JAL:
            imm = imm_j;
            break;

        case Ops::BRANCH:
            imm = imm_b;
            break;

        case Ops::OP:
            imm = vs2;
            break;

        case Ops::AUIPC:
        case Ops::LUI:
            imm = imm_u;
            break;

        case Ops::IMM:
        case Ops::LOAD:
        case Ops::JALR:
        case Ops::SYSTEM:
        case Ops::MISC:
            imm = imm_i;
            break;
            
        default:
            throw std::runtime_error(
                    Formatter() << "Unknown OP code: " <<  opcode);
    }

    
    bool pending_new_pc = (opcode == Ops::JAL || opcode == Ops::JALR) || (opcode == Ops::BRANCH && conditional(funct3, vs1, vs2));
    int32_t pend = arithmetics(
            arith_funct, 
            arith_left, 
            imm, 
            isAltMode(funct7, funct3, opcode));


    /* Handles system ops */
    if(opcode == Ops::SYSTEM){
        // I-Type instruction
        if(funct3 == Funct3::ECALL){
            std::cout << "Requested ECALL " <<  HEX(reg.regs[3]) << std::endl;
            if(reg.regs[3] > 1)
                throw std::runtime_error("Test has failed");
            else if(reg.regs[3] == 1)
                return false;
        }
    }

    
    /* Handles load and store ops */
    if(opcode == Ops::LOAD){
        switch(funct3){
            case Funct3::LB:
                pend = sign_extend(mem.readSegment(pend) & 0xFF, 8);
                break;
            case Funct3::LH:
                pend = sign_extend(mem.readSegment(pend) & 0xFFFF, 16); 
                break;
            case Funct3::LW:
                pend = mem.readSegment(pend);
                break;
            case Funct3::LBU:
                pend = mem.readSegment(pend) & 0xFF;
                break;
            case Funct3::LHU:
                pend = mem.readSegment(pend) & 0xFFFF;
                break;

            default:
                throw std::runtime_error("Unimplemented LOAD funct3");
        }
    }

    else if(opcode == Ops::STORE){
        uint8_t dat[4];
        switch(funct3){
            case Funct3::SB:
                std::memcpy(dat, &vs2, 1);
                mem.writeSegment(dat, 1, pend);
                break;
            case Funct3::SH:
                std::memcpy(dat, &vs2, 2);
                mem.writeSegment(dat, 2, pend);
                break;
            case Funct3::SW:
                std::memcpy(dat, &vs2, 4);
                mem.writeSegment(dat, 4, pend);
            default:
                throw std::runtime_error("Unimplemented STORE funct3");
        }
    }

    /*
    if(needRegWriteback(opcode)){
        std::cout << "writeback to register" << std::endl;
        reg.regs[rd] = pending_new_pc ?  vpc + 4 : pend;
        reg.regs[PC] = pending_new_pc ?  pend : vpc + 4;
    }
    */

    if(pending_new_pc){
        if(needRegWriteback(opcode))
            reg.regs[rd] = vpc + 4;
        reg.regs[PC] = pend;
    } else{
        if(needRegWriteback(opcode))
            reg.regs[rd] = pend;
        reg.regs[PC] = vpc + 4;
    }

    return true;
}
