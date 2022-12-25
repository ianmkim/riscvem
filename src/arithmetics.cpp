#include "arithmetics.hpp"

#include "funct3.hpp"
#include "utils.hpp"

#include <stdint.h>
#include <stdexcept>

int32_t arithmetics(Funct3 funct3, int x, int y, bool alt){
    switch(funct3){
        case Funct3::ADDI:
            if(alt) return x - y;
            return x + y;
            break;

        case Funct3::SLLI:
            return x << (y & 0x1f);
            break;

        case Funct3::SRLI:
            if(alt) {
                int sb = x >> 31;
                int out = x >> (y & 0x1f);
                out = out | (0xfffffff * sb) << (32 - (y & 0x1f));
                return out;
            } else return x >> (y & 0x1f);
            break;

        case Funct3::ORI:
            return x | y;
            break;

        case Funct3::XORI:
            return x ^ y;
            break;

        case Funct3::ANDI:
            return x & y; 
            break;

        case Funct3::SLT:
            return (int)(sign_extend(x, 32) < sign_extend(y, 32));
            break;

        case Funct3::SLTU:
            return (int)((x & 0xFFFFFFF) < (y & 0xFFFFFFF));
            break;

        default:
            throw std::runtime_error(
                    "Unimplemented Arithmetic Function. Please implement");
    }
}
