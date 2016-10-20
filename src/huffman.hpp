#ifndef ARCHIVER_HUFFMAN_HPP
#define ARCHIVER_HUFFMAN_HPP

#include <string>

class HuffmanArchiver {
public:
    static void archive(const std::string& input_file, const std::string& output_file);
    static void unzip(const std::string& input_file, const std::string& output_file);
};

#endif
