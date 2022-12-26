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

    void set(uint32_t indx, uint32_t val);

    uint32_t get(uint32_t indx);
    
    ~Regfile();
};

#endif 
