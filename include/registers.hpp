#ifndef REGISTERS_H
#define REGISTERS_H

#include <vector>
#include <string>

class Regfile{
public:
    uint32_t* regs;
    std::vector<std::string> regnames;

    Regfile();

    void dump();

    void reset();
    
    ~Regfile();
};

#endif 
