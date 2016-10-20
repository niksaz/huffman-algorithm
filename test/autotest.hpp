#ifndef archiver_autotest_hpp
#define archiver_autotest_hpp

class Autotester {
public:
    static void test_all();
    
    static void test_huffman();
    
    static void test_bitio_functions();
    static void test_bytereader();
    static void test_bytewriter();
    static void test_bit_functions();
    static void test_io_size_t();
    
    static void test_constants_values();
    
private:
    static void incorrect_behavior();
    static void correct_behavior();
};

#endif