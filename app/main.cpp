#include <iostream>

#include <elfio/elfio.hpp>

int main(int argc, char** argv){
    ELFIO::elfio reader;
    if(!reader.load("../rv32ui-p-add")){
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
              << std::endl;

        const char* p = reader.sections[i]->get_data();
    }
    
    return 0;
}
