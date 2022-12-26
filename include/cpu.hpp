#ifndef CPU_H
#define CPU_H

#include "memory.hpp"
#include "registers.hpp"

#include <string>

bool step(Memory &mem, Regfile &reg);

bool run(std::string filename);

#endif
