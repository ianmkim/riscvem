#ifndef CPU_H
#define CPU_H

#include "memory.hpp"
#include "registers.hpp"

bool step(Memory &mem, Regfile &reg);

#endif
