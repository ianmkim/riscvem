#include <iostream>

#include <elfio/elfio.hpp>

#include "utils.hpp"
#include "memory.hpp"
#include "registers.hpp"

int main(int argc, char** argv){
    ELFIO::elfio reader;
    if(!reader.load("../riscv-tests/isa/rv32ui-p-add")){
        std::cout << "Cannot find or process ELF file" << std::endl;
        return 2;
    }

    ELFIO::Elf_Half sec_num = reader.sections.size();
    std::cout << "Number of sections: " << sec_num << std::endl;
    for(int i = 0; i < sec_num; i++){
        const ELFIO::section* psec = reader.sections[i];
        std::cout << "  [" << i << "] "
              << psec->get_name()
              << "\t"
              << psec->get_size()
              << "\t"
              << HEX(psec->get_address())
              << std::endl;

        int int_addr = (int)psec->get_address();
        if(int_addr != 0x0){
            const unsigned char* p = (unsigned char*)reader.sections[i]->get_data();
            for (int e = 0; e < reader.sections[i]->get_size(); e+=4){
                int inst;
                std::memcpy(&inst, &p[e], 4);
                std::cout << std::hex << HEX(inst) << std::endl;
            }
            std::cout << std::endl;
        }
    }

    Regfile regfile;
    regfile.dump();
    
    return 0;
}
