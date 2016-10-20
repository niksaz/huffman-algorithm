#ifndef ARCHIVER_BITIO_HPP
#define ARCHIVER_BITIO_HPP

#include <fstream>
#include <string>

#include "constants.hpp"

class ByteReader {
public:
     ByteReader(const std::string& file_name);
    ~ByteReader();
    uint8_t read_bit();
    uint8_t read_byte();
    size_t read_size_t(int size);
    void complete_byte();
    size_t tell_complete_bytes();
    bool no_more_characters();
    void clear_flags();
    void seekg(std::ifstream::pos_type position);
    void close();
    
private:
    void read_next_byte();
    
    std::ifstream input_file;
    uint8_t buffer;
    size_t ptr = 0;
};

class ByteWriter {
public:
     ByteWriter(const std::string& file_name);
    ~ByteWriter();
    void write_bit(uint8_t bit);
    void write_byte(uint8_t byte);
    void write_size_t(size_t num, int size);
    void write_binary_string(const std::string& bs);
    void complete_byte();
    void close();
    size_t tell_complete_bytes();
    
private:
    void write_cur_byte();
    
    std::ofstream output_file;
    uint8_t buffer;
    size_t ptr = BITS_IN_BYTE;
};

#endif
