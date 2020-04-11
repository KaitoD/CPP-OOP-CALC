#include <chrono>
#include <iostream>

#include "../src/bigint64.hpp"
using calc::uint128_t;
using BI = calc::BigInt<uint128_t>;
int main() {
    BI a, b, x, y;
    std::cout << std::dec;
    uint64_t x1 = 100;
    auto c = calc::Factorial(x1);
    std::cout << "import math" << std::endl;
    std::cout << "print(math.factorial(" << x1 << ") == " << c << ")"
              << std::endl;
    c = BI(12345);
    std::cout << "print(12345**12345 == " << calc::Power(c, 12345) << ")"
              << std::endl;
    size_t test_count = 24;
    for (size_t i = 0; i < test_count; ++i) {
        c = calc::ExtGcdBin(a.GenRandom(3), b.GenRandom(5), &x, &y);
        std::cout << "print(" << c << " == math.gcd(" << a << ", " << b << "))"
                  << std::endl;
        std::cout << "print(" << c << " == " << a << " * " << x << " + " << b
                  << " * " << y << ")" << std::endl;
    }
    return 0;
}
