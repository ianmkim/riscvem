#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#define MEM_OFFSET 0x80000000

class Memory{
public:
    Memory(int size, int mem_offset);

    void writeSegment(const uint8_t* data, uint32_t datsize, uint32_t addr);

    void dumpAll();

    void dump(uint32_t start, uint32_t end);

    void reset();

    int32_t readSegment(uint32_t addr);

    ~Memory();

private:
    uint8_t* mem;
    uint32_t mem_offset;
    uint32_t mem_size;
};

#endif
