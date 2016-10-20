#include <iostream>

#include "../src/huffman.hpp"
#include "../src/bitio.hpp"
#include "../src/constants.hpp"
#include "huffman_test.hpp"
#include "autotest.hpp"

extern uint8_t get_bit(uint8_t value, size_t position);
extern void set_bit(uint8_t& value, size_t position, uint8_t bit);

using namespace std;

const string TEST_FILE_NAME = "../files/intermediate";

void Autotester::incorrect_behavior() {
    cout << "Incorrect behavior.\n";
    exit(EXIT_FAILURE);
}

void Autotester::correct_behavior() {
    cout << "OK: CORRECT BEHAVIOR.\n";
}

void Autotester::test_all() {
    test_huffman();
    test_bitio_functions();
    test_constants_values();
}

void Autotester::test_huffman() {
    cout << "testing huffman.\n";
    
    HuffmanArchiverTester::test_all();
}

void Autotester::test_bytereader() {
    cout << "testing the ByteReader class.\n";
    string model = "bitio\nByteReader? module enabled\n";
    ofstream output(TEST_FILE_NAME);
    output << model;
    output.close();
    
    ByteReader input(TEST_FILE_NAME);
    for (auto wanted : model) {
        if (input.no_more_characters()) {
            incorrect_behavior();
        }
        auto next_char = input.read_byte();
        if (next_char != wanted) {
            incorrect_behavior();
        }
    }
    
    if (!input.no_more_characters()) {
        incorrect_behavior();
    }
    
    correct_behavior();
}

void Autotester::test_bytewriter() {
    cout << "testing the ByteWriter class.\n";
    string model = "bitio\nByteWriter? module enabled\n";
    ByteWriter output(TEST_FILE_NAME);
    for (auto printing_char : model) {
        output.write_byte(printing_char);
    }
    output.close();
    
    ifstream input(TEST_FILE_NAME);
    string got((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    input.close();
    
    if (model != got) {
        incorrect_behavior();
    } else {
        correct_behavior();
    }
}

void Autotester::test_bit_functions() {
    cout << "testing bit setter and getter.\n";
    uint8_t byte = 255;
    for (size_t bit = 0; bit < BITS_IN_BYTE; bit++) {
        if (get_bit(byte, bit) != 1) {
            incorrect_behavior();
        }
    }
    for (size_t bit = 0; bit < BITS_IN_BYTE; bit++) {
        set_bit(byte, bit, 0);
    }
    for (size_t bit = 0; bit < BITS_IN_BYTE; bit++) {
        if (get_bit(byte, bit) != 0) {
            incorrect_behavior();
        }
    }
    correct_behavior();
}

void Autotester::test_io_size_t() {
    cout << "testing print_size_t and read_size_t\n";
    ByteWriter output(TEST_FILE_NAME);
    size_t model_file_size = 1234567;
    output.write_size_t(model_file_size, LOG_MAX_SIZE);
    output.close();
    
    ByteReader input(TEST_FILE_NAME);
    auto got_file_size = input.read_size_t(LOG_MAX_SIZE);
    input.close();
    
    if (model_file_size != got_file_size) {
        incorrect_behavior();
    } else {
        correct_behavior();
    }
}

void Autotester::test_bitio_functions() {
    cout << "testing bitio functions.\n";
    
    test_bytereader();
    test_bytewriter();
    test_bit_functions();
    test_io_size_t();
    
    cout << "\n";
}

void Autotester::test_constants_values() {
    cout << "testing constatns functions.\n";
    
    cout << "bits in byte: " << BITS_IN_BYTE << "\n";
    cout << "the alphabet size: " << ALPHABET << "\n";
    cout << "the maximum size: " << MAX_SIZE << "\n";
    cout << "the log of the maximum size: " << LOG_MAX_SIZE << ", it equals to " << (1 << LOG_MAX_SIZE) << "\n";
    
    cout << "\n";
}
