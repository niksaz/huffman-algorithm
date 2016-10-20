#ifndef ARCHIVER_CONSTANTS_HPP
#define ARCHIVER_CONSTANTS_HPP

#include <cstdint>

const size_t BITS_IN_BYTE = 8;
const size_t ALPHABET = 1 << BITS_IN_BYTE;
const size_t MAX_SIZE = 5 * 1024 * 1024;
extern const int LOG_MAX_SIZE;

#endif