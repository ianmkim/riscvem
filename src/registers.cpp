#include "registers.hpp"

#include "utils.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

Regfile::Regfile(){
    this->regs = (int*)malloc(33 * sizeof(int));
    if(this->regs == nullptr){
        throw std::runtime_error("Could not allocate enough memory for registers");
    } std::memset(this->regs, 0, 33 * sizeof(int));

    std::vector<std::string> names{
        "x0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
        "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7",
        "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11",
        "t3", "t4", "t5", "t6", "PC",
    };
    regnames.assign(names.begin(), names.end());
}


void Regfile::dump(){
    std::cout << "REGISTER DUMP: " << std::endl;
    for(int i = 0; i < 33; i++){
        if(i != 0 && i % 8 == 0){
            std::cout << std::endl;
        } std::cout << "\t" << this->regnames[i] << ": " << HEX(this->regs[i]);
    } std::cout << std::endl;
}


void Regfile::reset(){
    std::memset(this->regs, 0, 33 * sizeof(int));
}


Regfile::~Regfile(){
    if(this->regs != nullptr)
        free(this->regs);
}
