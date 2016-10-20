#include "autotest.hpp"
#include "huffman_test.hpp"

#include <cstdlib>

using namespace std;

const unsigned int SEED = 7;

int main(void) {
    srand(SEED);
    Autotester::test_all();
    return EXIT_SUCCESS;
}
