#include <fstream>
#include <iostream>
#include <string>

#include "../src/huffman.hpp"
#include "../src/constants.hpp"
#include "huffman_test.hpp"

using namespace std;

const string INPUT_FILE_NAME = "../files/input";
const string INTERMEDIATE_FILE_NAME = "../files/intermediate";
const string OUTPUT_FILE_NAME = "../files/output";
const string HENRY_FILE_NAME = "../files/henry.txt";

const size_t DEFAULT_FILE_SIZE = 8192;

double get_current_time() {
    return static_cast<double>(clock()) / CLOCKS_PER_SEC;
}

void HuffmanArchiverTester::test_all() {
    test_huge_file();
    test_empty_file();
    test_one_letter_file();
    test_ten_letter_file();
    test_real_text();
}

void HuffmanArchiverTester::compare_files(const string& file_name, const string& model) {
    auto incorrect_result = []() {
        cout << "output file differs from model text.\n";
        exit(EXIT_FAILURE);
    };

    ifstream input;
    input.open(file_name, ios::in | ios::binary);
    for (auto wanted_char : model) {
        char next_char;
        input.read(&next_char, 1);
        if (input.eof()) {
            incorrect_result();
        }
        if (next_char != wanted_char) {
            incorrect_result();
        }
    }
    if (input.peek() != ifstream::traits_type::eof()) {
        incorrect_result();
    }
    input.close();
    cout << "OK: CORRECT RESULT.\n\n";
}

void HuffmanArchiverTester::run_test(const string& model) {
    ofstream out;
    out.open(INPUT_FILE_NAME);
    out << model;
    out.close();

    auto compressing_start_time = get_current_time();
    cout << "starting the compression.\n";
    HuffmanArchiver::archive(INPUT_FILE_NAME, INTERMEDIATE_FILE_NAME);
    auto compressing_finish_time = get_current_time();
    cout << "compressing finished in: " << compressing_finish_time - compressing_start_time << "\n";
    HuffmanArchiver::unzip(INTERMEDIATE_FILE_NAME, OUTPUT_FILE_NAME);
    cout << "starting the decompression.\n";
    auto decompressing_finish_time = get_current_time();
    cout << "decompressing finished in: " << decompressing_finish_time - compressing_finish_time << "\n";
    
    compare_files(OUTPUT_FILE_NAME, model);
}

void HuffmanArchiverTester::test_huge_file() {
    cout << "running test with a huge input size.\n";
    string model;
    for (size_t built_prefix = 0; built_prefix < MAX_SIZE; built_prefix++) {
        model += 'a' + rand() % ALPHABET;
    }
    run_test(model);
}

void HuffmanArchiverTester::test_empty_file() {
    cout << "running test with the empty file.\n";
    string model;
    run_test(model);
}

void HuffmanArchiverTester::test_one_letter_file() {
    cout << "running test with the file which contains only one letter.\n";
    string model;
    for (size_t built_prefix = 0; built_prefix < DEFAULT_FILE_SIZE; built_prefix++) {
        model += '?';
    }
    run_test(model);
}

void HuffmanArchiverTester::test_ten_letter_file() {
    cout << "running test with the file which contains ten letters.\n";
    string model;
    for (size_t built_prefix = 0; built_prefix < DEFAULT_FILE_SIZE; built_prefix++) {
        model += 'a' + rand() % 10;
    }
    run_test(model);
}

void HuffmanArchiverTester::test_real_text() {
    cout << "running test which is The First part of King Henry the Fourth.\n";
    ifstream input(HENRY_FILE_NAME);
    string model((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
    input.close();
    run_test(model);
}
