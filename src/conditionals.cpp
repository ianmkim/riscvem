#include "conditionals.hpp"

#include "funct3.hpp"
#include "utils.hpp"

#include <stdexcept>

bool conditional(Funct3 funct3, int32_t vs1, int32_t vs2){
    bool ret = false;
    switch(funct3){
        case Funct3::BEQ:
            ret = vs1 == vs2;
            break;

        case Funct3::BNE:
            ret = vs1 != vs2;
            break;
            
        case Funct3::BLT:
            ret = sign_extend(vs1, 32) < sign_extend(vs2, 32);
            break;

        case Funct3::BGE:
            ret = sign_extend(vs1, 32) >= sign_extend(vs2, 32);
            break;

        case Funct3::BLTU:
            ret = vs1 < vs2;
            break;

        case Funct3::BGEU:
            ret = vs1 >= vs2;
            break;

        default:
            throw std::runtime_error(
                "Tried to call a conditional function that was not implemented");
    }

    return ret;
}
