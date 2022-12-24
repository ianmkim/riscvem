#include "memory.hpp"

#include <stdexcept>
#include <cstring>

Memory::Memory(int size, int mem_offset=0x80000000){
    if(size <= 0)
        throw std::invalid_argument("Size of the memory must be at least 1 bytes");

    this->mem_offset = mem_offset;
    this->mem_size = size;
    this->mem = (char8_t*)malloc(sizeof(char8_t)*size);

    if(this->mem == nullptr)
        throw std::runtime_error("Could not allocate enough memory for given size");

    std::memset(this->mem, (char8_t)0x00, size);
}


void Memory::writeSegment(const char8_t* data, int datsize, int addr){
    addr -= this->mem_offset;
    if(addr < 0 || addr >= this->mem_size)
        throw std::runtime_error("Tried to access an illegal memory address");
    if(addr + datsize >= this->mem_size)
        throw std::runtime_error("Tried to write a segment that causes an overflow");
    std::memcpy(&this->mem[addr], data, datsize);
}


int Memory::readSegment(int addr){
    addr -= this->mem_offset;
    if(addr < 0 || addr >= this->mem_size)
        throw std::runtime_error("Tried to access an illegal memory address");
    int val;
    memcpy(&val, &this->mem[addr], 4);
    return val;
}


Memory::~Memory(){
    if(this->mem != nullptr)    
        free(this->mem);
}
