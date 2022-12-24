#include "utils.hpp"


/*
 * sign_extend
 * 
 * increases the number of bits in a binary number while preserving the
 * number's sign and value
 * 
 * @param x 32 bit binary string to extend
 * @param l length of the resultant binary string
 *
 * @return a binary string that has the length of l that has the same sign and
 * value as x parameter
 */
int32_t sign_extend(int32_t x, int l){
    if(x >> (l - 1) == 1)
        return -((1 << l) - x);
    return x;
}


/*
 * get_bits
 *
 * get bits in a little endian encoded 32 bit string (encoded as an int)
 * given a start and end position in int type.
 * 
 * @param ins data to get bits from
 * @param s start position
 * @param e end positio
 * 
 * @return integer representation of the bits
 */
int32_t get_bits(int32_t ins, int s, int e){
    return (ins >> e) & ((1 << (s - e + 1)) - 1);
}
