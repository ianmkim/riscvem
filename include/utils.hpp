#ifndef UTILS_H
#define UTILS_H

#include <iomanip>
#include <sstream>

#define HEX( x ) "0x" << std::setw(8) << std::setfill('0') << std::hex << (int)( x )
#define HEX8( x ) "0x" << std::setw(2) << std::setfill('0') << std::hex << (int)( x )


class Formatter
{
public:
    Formatter() {}
    ~Formatter() {}

    template <typename Type>
    Formatter & operator << (const Type & value)
    {
        stream_ << value;
        return *this;
    }

    std::string str() const         { return stream_.str(); }
    operator std::string () const   { return stream_.str(); }

    enum ConvertToString 
    {
        to_str
    };
    std::string operator >> (ConvertToString) { return stream_.str(); }

private:
    std::stringstream stream_;

    Formatter(const Formatter &);
    Formatter & operator = (Formatter &);
};

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
int32_t sign_extend(int32_t x, int l);

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
int32_t get_bits(int32_t ins, int s, int e);

#endif
