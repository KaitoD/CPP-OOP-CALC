#include <chrono>
#include <iostream>

#include "../src/bigint64.hpp"
using calc::uint128_t;
using BI = calc::BigInt<uint128_t>;
void do_test(const BI& a, const BI& b) {
    std::cout << std::hex << std::showbase;
    uint64_t t = std::rand();
    std::cout << "print(" << a << " * " << t << " == " << a * t << ")"
              << std::endl;
    t = std::rand();
    std::cout << "print(" << b << " * " << t << " == " << b * t << ")"
              << std::endl;
    std::cout << std::endl;
    std::cout << "print(" << a << " * " << b << " == " << a.MNTMul(a, b) << ")"
              << std::endl;
    std::cout << "print(" << a << " * " << b << " == " << a.RMNTMul(a, b) << ")"
              << std::endl;
    std::cout << "print(" << b << " * " << a << " == " << a.MNTMul(b, a) << ")"
              << std::endl;
    std::cout << "print(" << b << " * " << a << " == " << a.RMNTMul(b, a) << ")"
              << std::endl;
    std::cout << "print(" << a << " * " << a << " == " << a.Square(a) << ")"
              << std::endl;
    std::cout << "print(" << b << " * " << b << " == " << a.Square(b) << ")"
              << std::endl;
    std::cout << "print(" << a << " * " << b << " == " << a.RMNTMulUB(a, b)
              << ")" << std::endl;
    std::cout << "print(" << b << " * " << a << " == " << a.RMNTMulUB(b, a)
              << ")" << std::endl;
    std::cout << std::endl;
    std::cout << "print(" << std::hex << std::showbase << a
              << " == " << std::dec << a << ")" << std::endl;
    std::cout << "print(" << std::hex << std::showbase << b
              << " == " << std::dec << b << ")" << std::endl;
    std::cout << std::endl;
    BI c;
    std::cout << std::hex << std::showbase;
    if (a.Sign() == b.Sign()) {
        std::cout << "print(" << a << " // " << b << " == " << a.DivD(a, b, &c)
                  << ")" << std::endl;
    }
    std::cout << "print(" << a << " == " << b << " * " << a.DivD(a, b, &c);
    std::cout << " + " << c << ")" << std::endl;
    if (a.Sign() == b.Sign()) {
        std::cout << "print(" << a << " // " << b << " == " << a.DivR(a, b, &c)
                  << ")" << std::endl;
    }
    std::cout << "print(" << a << " == " << b << " * " << a.DivR(a, b, &c);
    std::cout << " + " << c << ")" << std::endl;
}
int main() {
    std::srand(unsigned(time(nullptr)));
    BI a(0), b;
    constexpr int len1 = 7, len2 = 5, len3 = 1;
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
