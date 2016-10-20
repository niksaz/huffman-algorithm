#include <iostream>
#include <string>
#include <functional>

#include "huffman.hpp"

using namespace std;

const char* COMPRESS_FLAG = "-c";
const char* DECOMPRESS_FLAG = "-u";

const char* SHORT_INPUT_FILE_FLAG = "-f";
const char* FULL_INPUT_FILE_FLAG  = "--file";

const char* SHORT_OUTPUT_FILE_FLAG = "-o";
const char* FULL_OUTPUT_FILE_FLAG  = "--output";

void command_line_parsing_error(const string& specific_error) {
    cerr << specific_error << "\n";
    exit(EXIT_FAILURE);
}

int main(int argc, const char* argv[]) {
    auto found_compress = false;
    auto found_decompress = false;
    
    string input_file = "";
    string output_file = "";
    
    for (int index = 1; index < argc; index++) {
        auto differ_from_current = bind(strcmp, argv[index], placeholders::_1);
        if (!differ_from_current(COMPRESS_FLAG)) {
            found_compress = true;
        } else if (!differ_from_current(DECOMPRESS_FLAG)) {
            found_decompress = true;
        } else if (!differ_from_current(SHORT_INPUT_FILE_FLAG) ||
                   !differ_from_current(FULL_INPUT_FILE_FLAG)) {
            if (index + 1 == argc) {
                command_line_parsing_error("Provide a path after the input file flag.");
            }
            input_file = argv[++index];
        } else if (!differ_from_current(SHORT_OUTPUT_FILE_FLAG) ||
                   !differ_from_current(FULL_OUTPUT_FILE_FLAG)) {
            if (index + 1 == argc) {
                command_line_parsing_error("Provide a path after the output file flag.");
            }
            output_file = argv[++index];
        } else {
            command_line_parsing_error("Unrecognized argument: " + string(argv[index]) + ".");
        }
    }
    
    if ((!found_compress && !found_decompress) || (found_compress && found_decompress)) {
        command_line_parsing_error("Choose either to compress or to decompress a file.");
    }
    if (input_file == "") {
        command_line_parsing_error("Specify an input file.");
    }
    if (output_file == "") {
        command_line_parsing_error("Specify an output file.");
    }
    
    try {
        auto action = (found_compress ? HuffmanArchiver::archive : HuffmanArchiver::unzip);
        action(input_file, output_file);
    } catch (exception any) {
        cerr << any.what() << "\n";
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
