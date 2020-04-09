#include <chrono>
#include <iostream>

#include "../src/bigint64.hpp"
using calc::uint128_t;
using BI = calc::BigInt<uint128_t>;
void do_test(const BI& a, const BI& b) {
    std::cout << std::hex << std::showbase;
    std::cout << "print(" << a << " * " << b << " == " << a.MNTMul(a, b) << ")"
              << std::endl;
    std::cout << "print(" << a << " * " << b << " == " << a.RMNTMul(a, b) << ")"
              << std::endl;
    std::cout << std::endl;
    std::cout << "print(" << std::hex << std::showbase << a
              << " == " << std::dec << a << ")" << std::endl;
    std::cout << "print(" << std::hex << std::showbase << b
              << " == " << std::dec << b << ")" << std::endl;
}
int main() {
    BI a(0), b;
    constexpr int len1 = 3, len2 = 3, len3 = 3;
    do_test(a.GenRandom(len1), b.GenRandom(len2));
    std::cout << "print()" << std::endl;
    do_test(a.GenRandom(len1).ToOpposite(), b.GenRandom(len2));
    std::cout << "print()" << std::endl;
    do_test(a.GenRandom(len1), b.GenRandom(len2).ToOpposite());
    std::cout << "print()" << std::endl;
    do_test(a.GenRandom(len1).ToOpposite(), b.GenRandom(len2).ToOpposite());
    std::cout << "print()" << std::endl;
    do_test(a.GenRandom(len3), b.GenRandom(len3));
    std::cout << "print()" << std::endl;
    do_test(a.GenRandom(len3).ToOpposite(), b.GenRandom(len3));
    std::cout << "print()" << std::endl;
    do_test(a.GenRandom(len3), b.GenRandom(len3).ToOpposite());
    std::cout << "print()" << std::endl;
    do_test(a.GenRandom(len3).ToOpposite(), b.GenRandom(len3).ToOpposite());
    return 0;
}
