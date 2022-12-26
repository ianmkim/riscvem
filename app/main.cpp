#include <iostream>

#include <elfio/elfio.hpp>

#include "utils.hpp"
#include "memory.hpp"
#include "registers.hpp"
#include "cpu.hpp"

int main(int argc, char** argv){
    // 64k memory starting at 0x80000000
    Memory mem(0x4000, 0x80000000); 
    Regfile reg;

    ELFIO::elfio reader;
    if(!reader.load("../riscv-tests/isa/rv32ui-p-add")){
        std::cout << "Cannot find or process ELF file" << std::endl;
        return 2;
    }

    ELFIO::Elf_Half sec_num = reader.sections.size();
    std::cout << "Number of sections: " << sec_num << std::endl;
    for(int i = 0; i < sec_num; i++){
        mem.reset();
        reg.reset();

        const ELFIO::section* psec = reader.sections[i];
        std::cout << "  [" << i << "] "
              << psec->get_name()
              << "\t"
              << psec->get_size()
              << "\t"
              << HEX(psec->get_address())
              << std::endl;

        int int_addr = (int)psec->get_address();
        std::cout << HEX(int_addr) << std::endl;
        if(int_addr != 0x0){
            const uint8_t* p = (uint8_t*)reader.sections[i]->get_data();
            mem.writeSegment(p, reader.sections[i]->get_size(), int_addr);

            reg.regs[PC] = MEM_OFFSET;
            int instruction_count = 0;
            while(step(mem, reg)){
                std::cout << std::dec << (int)instruction_count << " instructions ran" << std::endl;
                instruction_count++;
            }

            std::cout << "TEST SUCCEDED" << std::endl;
            std::cout << " ran " << instruction_count << " instructions" << std::endl;
        }
    }

    Regfile regfile;
    regfile.dump();
    
    return 0;
}
