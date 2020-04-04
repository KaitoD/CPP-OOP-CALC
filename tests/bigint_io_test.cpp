#include <chrono>
#include <iostream>

#include "../src/bigint.hpp"
int main() {
    calc::BigInt<uint8_t> a(12345);
    a = calc::Power(a, 12345ul);
    std::cout << "print(12345**12345 == " << a << ")" << std::endl;
    return 0;
}
