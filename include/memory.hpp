#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define MEM_OFFSET 0x80000000

class Memory{
public:
    Memory(int size, int mem_offset);

    void writeSegment(const uint8_t* data, int datsize, int addr);

    void dump(uint32_t start, uint32_t end);

    int readSegment(int addr);

    ~Memory();

private:
    uint8_t* mem;
    int mem_offset;
    int mem_size;
};

#endif
