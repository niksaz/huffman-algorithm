#include "bitio.hpp"

using namespace std;

uint8_t get_bit(uint8_t value, size_t position) {
    return (value & (1 << position)) == 0 ? 0 : 1;
}

void set_bit(uint8_t& value, size_t position, uint8_t bit) {
    value &= ~(1 << position);
    value |= bit * (1 << position);
}

ByteReader::ByteReader(const string& file_name) {
    input_file.open(file_name, ios::in | ios::binary);
    if (!input_file.is_open()) {
        throw ios_base::failure("Couldn't open a file with the name:" + file_name + ".");
    }
}

ByteReader::~ByteReader() {
    close();
}

uint8_t ByteReader::read_bit() {
    if (ptr == 0) {
        read_next_byte();
    }
    auto res = get_bit(buffer, --ptr);
    return res;
}

uint8_t ByteReader::read_byte() {
    uint8_t res = 0;
    for (int position = BITS_IN_BYTE - 1; position >= 0; position--) {
        set_bit(res, position, read_bit());
    }
    return res;
}

size_t ByteReader::read_size_t(int size) {
    size_t res = 0;
    for (int res_prefix_computed = 0; res_prefix_computed < size; res_prefix_computed++) {
        res <<= 1;
        res += read_bit();
    }
    return res;
}

void ByteReader::complete_byte() {
    while (ptr != 0) {
        read_bit();
    }
}

size_t ByteReader::tell_complete_bytes() {
    return static_cast<size_t>(input_file.tellg()) - (ptr == 0 ? 0 : 1);
}

bool ByteReader::no_more_characters() {
    return input_file.peek() == ifstream::traits_type::eof();
}

void ByteReader::read_next_byte() {
    input_file.read(reinterpret_cast<char*>(&buffer), 1);
    if (input_file.eof()) {
        throw ios_base::failure("Unexpected end of the file.");
    }
    ptr = BITS_IN_BYTE;
}

void ByteReader::clear_flags() {
    input_file.clear();
}

void ByteReader::seekg(std::ifstream::pos_type position) {
    input_file.seekg(position);
}

void ByteReader::close() {
    input_file.close();
}

ByteWriter::ByteWriter(const string& file_name) {
    output_file.open(file_name, ios::out | ios::binary);
    if (!output_file.is_open()) {
        throw ios_base::failure("Couldn't open a file with the name:" + file_name + ".");
    }
}

ByteWriter::~ByteWriter() {
    close();
}

void ByteWriter::write_bit(uint8_t bit) {
    set_bit(buffer, --ptr, bit);
    if (ptr == 0) {
        write_cur_byte();
    }
}

void ByteWriter::write_byte(uint8_t byte) {
    for (int position = BITS_IN_BYTE - 1; position >= 0; position--) {
        write_bit(get_bit(byte, position));
    }
}

void ByteWriter::write_size_t(size_t num, int size) {
    for (int printing_bit = size - 1; printing_bit >= 0; printing_bit--) {
        write_bit((num & (1 << printing_bit)) == 0 ? 0 : 1);
    }
}

void ByteWriter::write_binary_string(const string& bs) {
    for (auto c : bs) {
        write_bit(c - '0');
    }
}

void ByteWriter::complete_byte() {
    while (ptr != BITS_IN_BYTE) {
        write_bit(0);
    }
}

size_t ByteWriter::tell_complete_bytes() {
    return static_cast<size_t>(output_file.tellp());
}

void ByteWriter::close() {
    complete_byte();
    output_file.close();
}

void ByteWriter::write_cur_byte() {
    output_file.write(reinterpret_cast<char*>(&buffer), 1);
    ptr = BITS_IN_BYTE;
}
