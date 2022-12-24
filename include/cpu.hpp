#ifndef CPU_H
#define CPU_H

#include "memory.hpp"
#include "registers.hpp"

#define PC 32

bool step(Memory &mem, Regfile &reg);

#endif
