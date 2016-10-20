#ifndef archiver_huffman_test_hpp
#define archiver_huffman_test_hpp

#include <string>

class HuffmanArchiverTester {
public:
    static void run_test(const std::string& model);
    
    static void test_all();
    static void test_huge_file();
    static void test_empty_file();
    static void test_one_letter_file();
    static void test_ten_letter_file();
    static void test_real_text();
    
private:
    static void compare_files(const std::string& file_name, const std::string& model);
};

#endif