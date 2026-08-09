#include <cassert>
#include "../src/util/sum.hpp"


void test_sum_positive_numbers() {
    int a = 5;
    int b = 7;

    int expected = 12;

    assert(sum(a, b) == expected);
}

