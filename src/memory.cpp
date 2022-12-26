#include "memory.hpp"

#include "utils.hpp"

#include <stdexcept>
#include <cstring>
#include <stdint.h>
#include <iostream>

Memory::Memory(int size, int mem_offset=0x80000000){
    if(size <= 0)
        throw std::invalid_argument("Size of the memory must be at least 1 bytes");

    this->mem_offset = mem_offset;
    this->mem_size = size;
    this->mem = (uint8_t*)malloc(sizeof(uint8_t)*size);

    if(this->mem == nullptr)
        throw std::runtime_error("Could not allocate enough memory for given size");

    std::memset(this->mem, (uint8_t)0x00, size);
}


void Memory::dumpAll(){
    for(int i = 0; i < this->mem_size; i++){
        std::cout << HEX(i) << ": " << HEX8(this->mem[i]) << std::endl;
    }
}

void Memory::dump(uint32_t start, uint32_t end){
    for(uint32_t i = start - this->mem_offset; i < end - this->mem_offset; i++) {
        std::cout << HEX(i) << ": " << HEX8(this->mem[i]) << std::endl;
    }
}


void Memory::reset(){
    std::memset(this->mem, 0, this->mem_size);
}


void Memory::writeSegment(const uint8_t* data, uint32_t datsize, uint32_t addr){
    addr -= this->mem_offset;
    if(addr < 0 || addr >= this->mem_size)
        throw std::runtime_error("Tried to access an illegal memory address");
    if(addr + datsize >= this->mem_size)
        throw std::runtime_error("Tried to write a segment that causes an overflow");
    std::memcpy(&this->mem[addr], data, datsize);
}


int32_t Memory::readSegment(uint32_t addr){
    addr -= this->mem_offset;
    if(addr < 0 || addr >= this->mem_size)
        throw std::runtime_error("Tried to access an illegal memory address");
    int32_t val = 0;
    std::memcpy(&val, &this->mem[addr], 4);
    return val;
}


Memory::~Memory(){
    if(this->mem != nullptr)    
        free(this->mem);
}
