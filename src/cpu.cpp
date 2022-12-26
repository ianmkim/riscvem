#include "cpu.hpp"

#include <stdexcept>
#include <string>
#include <elfio/elfio.hpp>

#include "memory.hpp"
#include "registers.hpp"
#include "utils.hpp"

#include "conditionals.hpp"
#include "arithmetics.hpp"

#include "ops.hpp"
#include "funct3.hpp"


bool handleSyscall(Regfile &reg, Funct3 funct3);

void handleMemStore(Memory& mem, Funct3 funct3, uint32_t pend, uint32_t vs2);
   
uint32_t handleMemRead(Memory &mem, Funct3 funct3);

uint32_t parseImm(uint32_t ins);

bool isAltMode(uint32_t funct7, Funct3 funct3, Ops opcode);

bool needRegWriteback(Ops opcode);


bool run(std::string filename){
    // allocate 64K memory starting at 0x80000000 address
    // 64 bit memory addressing. No virtual memory
    Memory mem(0x4000, 0x80000000);

    // instantiates registers. Starts off as all zeros. 33
    // 32 bit wide registers including program pointer at 
    // index 32;
    Regfile reg;

    // loads up an ELF artifact which includes the assembly
    // instructions in raw bytes.
    ELFIO::elfio reader;
    if(!reader.load(filename)){
        std::cout << "Cannot find or process ELF file" << std::endl;
        return false;
    }
    
    ELFIO::Elf_Half sec_num = reader.sections.size();
    std::cout << "Number of sections: " << sec_num << std::endl;
    for(int i = 0; i < sec_num; i++){
        const ELFIO::section* psec = reader.sections[i];
        uint32_t int_addr = (uint32_t)psec->get_address();

        if(int_addr != 0){
            const uint8_t* p = (uint8_t*)reader.sections[i]->get_data();
            mem.writeSegment(p, reader.sections[i]->get_size(), int_addr);

            std::cout << "Wrote " << std::dec << reader.sections[i]->get_size() << " bytes to " << HEX(int_addr) << std::endl;
        }
    }

    std::cout << PC << std::endl;

    reg.regs[PC] = MEM_OFFSET;
    int instruction_count = 0;
    while(step(mem, reg)){
        std::cout << std::dec << (int)instruction_count << " instructions ran" << std::endl;
        instruction_count++;
    }

    std::cout << "TEST SUCCEDED" << std::endl;
    std::cout << " ran " << instruction_count << " instructions" << std::endl;
    
    return true;
}

bool step(Memory &mem, Regfile &reg){
    //mem.dumpAll();
    mem.dump(reg.get(PC), reg.get(PC) + 10);
    reg.dump();

    uint32_t ins = mem.readSegment(reg.get(PC));
    Ops opcode = (Ops)get_bits(ins, 6, 0);
    std::cout << reg.get(PC) << " " << HEX(ins) << " " << HEX( get_bits(ins, 6, 0) ) << std::endl;

    // a lot of this parsing is redundant, depending on the instruction
    // type, we will not need certain variables. However, overhead is small
    // and makes the actual computation much easier.

    // load funct3
    Funct3 funct3 = (Funct3)get_bits(ins, 14, 12);
    // load funct7
    uint32_t funct7 = get_bits(ins, 31, 25);

    uint32_t rd = get_bits(ins, 11, 7);
    uint32_t vs1 = reg.get(get_bits(ins, 19, 15));
    uint32_t vs2 = reg.get(get_bits(ins, 24, 20));
    uint32_t vpc = reg.get(PC);

    uint32_t arith_left = (opcode == Ops::JAL || 
                            opcode == Ops::BRANCH || 
                            opcode == Ops::AUIPC) ? vpc : (opcode == Ops::LUI ? 0 : vs1);
    Funct3 arith_funct = (opcode == Ops::OP || opcode == Ops::IMM) ? funct3 : Funct3::ADD;
 
    // load the appropriate immediate value depending on the opcode
    int32_t imm = (opcode == Ops::OP) ? vs2 : parseImm(ins);
    
    bool pending_new_pc = (opcode == Ops::JAL || opcode == Ops::JALR) || (opcode == Ops::BRANCH && conditional(funct3, vs1, vs2));
    uint32_t pend = arithmetics(
            arith_funct, 
            arith_left, 
            imm, 
            isAltMode(funct7, funct3, opcode));


    /* Handles system ops */
    if(opcode == Ops::SYSTEM){
        if(!handleSyscall(reg, funct3))
            return false;
    }
    
    /* Handles load and store ops */
    if(opcode == Ops::LOAD){
        pend = handleMemRead(mem, funct3);
    }

    else if(opcode == Ops::STORE){
        handleMemStore(mem, funct3, pend, vs2);
    }

    bool need_write = needRegWriteback(opcode);
    if(pending_new_pc){
        if(need_write)
            reg.set(rd, vpc + 4);
        reg.set(PC, pend);
    } else{
        if(need_write)
            reg.set(rd, pend);
        reg.set(PC, vpc + 4);
    }

    return true;
}


/***
 *  BEGINNING OF UTILITY FUNCTIONS
 ***/

bool handleSyscall(Regfile &reg, Funct3 funct3){
    // I-Type instruction
    if(funct3 == Funct3::ECALL){
        uint32_t gp_reg = reg.get(3);
        std::cout << "Requested ECALL " <<  HEX(gp_reg) << std::endl;
        if(gp_reg > 1)
            throw std::runtime_error("Test has failed");
        else if(gp_reg == 1)
            return false;
    }
    return true;
}

void handleMemStore(Memory& mem, Funct3 funct3, uint32_t pend, uint32_t vs2){
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

uint32_t handleMemRead(Memory &mem, Funct3 funct3){
    uint32_t pend;
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

    return pend;
}

uint32_t parseImm(uint32_t ins){
    uint32_t imm;
    Ops opcode = (Ops)get_bits(ins, 6, 0);
    switch(opcode){
        case Ops::STORE:
            // IMM type s
            imm = sign_extend((get_bits(ins, 31, 25) << 5)  | 
                                get_bits(ins, 11, 7), 12);
            break;

        case Ops::JAL:
            // IMM type j
            imm = sign_extend((get_bits(ins, 32, 31) << 20) |
                        (get_bits(ins, 30, 21) << 1)  |
                        (get_bits(ins, 21, 20) << 11) |
                        (get_bits(ins, 19, 12) << 12), 21);
            break;

        case Ops::BRANCH:
            // IMM type b
            imm = sign_extend((get_bits(ins, 32, 31) << 12) | 
                        (get_bits(ins, 30, 25) << 5)  |
                        (get_bits(ins, 11, 8)  << 1)  |
                        (get_bits(ins, 8, 7)   << 11), 13);
            break;

        case Ops::AUIPC:
        case Ops::LUI:
            // IMM type U
            imm = sign_extend(get_bits(ins, 31, 12) << 12, 32);
            break;

        case Ops::IMM:
        case Ops::LOAD:
        case Ops::JALR:
        case Ops::SYSTEM:
        case Ops::MISC:
            // IMM type i
            imm = sign_extend(get_bits(ins, 31, 20), 12); 
            break;
            
        default:
            throw std::runtime_error(Formatter() << "Unknown OP code: " <<  opcode);
    }
    return imm;
}

bool isAltMode(uint32_t funct7, Funct3 funct3, Ops opcode){
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