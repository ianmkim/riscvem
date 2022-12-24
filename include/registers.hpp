#ifndef REGISTERS_H
#define REGISTERS_H

#include <vector>
#include <string>

class Regfile{
public:
    int* regs;
    std::vector<std::string> regnames;

    Regfile();

    void dump();
    
    ~Regfile();
};

#endif 
