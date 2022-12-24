#ifndef MEMORY_H
#define MEMORY_H

class Memory{
public:
    Memory(int size, int mem_offset);

    void writeSegment(const char8_t* data, int datsize, int addr);

    int readSegment(int addr);

    ~Memory();

private:
    char8_t * mem;
    int mem_offset;
    int mem_size;
};

#endif
