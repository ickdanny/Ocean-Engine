#ifndef ZMATH_BITWISE_H
#define ZMATH_BITWISE_H

#include <stdint.h>
#include <stdbool.h>

/* Evaluates to true IFF machine is big endian */
/* from StackOverflow user caf */
#define isMachineBigEndian \
    (!*(unsigned char *)&(uint16_t){1})

/* Evaluates to true IFF machine is little endian */
#define isMachineLittleEndian (!isMachineBigEndian)

/* Swaps endianness of a short */
uint16_t byteSwap16(uint16_t i);

/* Swaps endianness of an int */
uint32_t byteSwap32(uint32_t i);

/* Swaps endianness of a long */
uint64_t byteSwap64(uint64_t i);

/* Converts a little endian short for the CPU */
uint16_t fromLittleEndian16(uint16_t i);

/* Converts little endian int for the CPU */
uint32_t fromLittleEndian32(uint32_t i);

/* Converts little endian long for the CPU */
uint64_t fromLittleEndian64(uint64_t i);

/* Converts a big endian short for the CPU */
uint16_t fromBigEndian16(uint16_t i);

/* Converts big endian int for the CPU */
uint32_t fromBigEndian32(uint32_t i);

/* Converts big endian long for the CPU */
uint64_t fromBigEndian64(uint64_t i);

/* Gets the ith byte from the given value */
#define getByte(value, i) \
    ((uint8_t)((value >> ((i) * 8)) & 0xFF))

/* Gets the ith bit from the given value as a bool */
#define getBit(value, i) (((value) >> (i)) & 1)

/* Sets the ith bit in the given variable */
#define setBit(variable, i) ((variable) |= (1 << (i)))

/* Unsets the ith bit in the given variable */
#define unsetBit(variable, i) \
    /* flips all bits, sets bit, then unflips */ \
    do{ \
        (variable) = ~(variable); \
        setBit(variable, i); \
        (variable) = ~(variable); \
    } while(false)

/* Flips the ith bit in the given variable */
#define flipBit(variable, i) \
    ((variable) ^= (1 << (i)))

#endif