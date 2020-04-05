#include <iostream>

#include "../src/bigint.hpp"
// test different multiplication
using IntT = uint16_t;
void do_test(calc::BigInt<IntT>& a, calc::BigInt<IntT>& b) {
    calc::BigInt<IntT> c;
    std::cout << std::hex << std::showbase;
    std::cout << a << " == " << b << " * " << a.DivAlgA(a, b, &c);
    std::cout << " + " << c << std::endl;
    std::cout << a << " == " << b << " * " << a.DivAlgB(a, b, &c);
    std::cout << " + " << c << std::endl;
    std::cout << a << " == " << b << " * " << a.DivRecursive(a, b, &c);
    std::cout << " + " << c << std::endl;

    std::cout << std::dec;
    std::cout << a << " == " << b << " * " << a.DivAlgA(a, b, &c);
    std::cout << " + " << c << std::endl;
    std::cout << a << " == " << b << " * " << a.DivAlgB(a, b, &c);
    std::cout << " + " << c << std::endl;
    std::cout << a << " == " << b << " * " << a.DivRecursive(a, b, &c);
    std::cout << " + " << c << std::endl;
}
void do_more_test(calc::BigInt<IntT>& a, calc::BigInt<IntT>& b) {
    calc::BigInt<IntT> c;
    std::cout << std::hex << std::showbase;
    std::cout << a << " // " << b << " == " << a.PlainDiv(a, b, &c)
              << std::endl;
    std::cout << a << " % " << b << " == " << c << std::endl;

    std::cout << std::dec;
    std::cout << a << " // " << b << " == " << a.PlainDiv(a, b, &c)
              << std::endl;
    std::cout << a << " % " << b << " == " << c << std::endl;
}
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    calc::BigInt<IntT> a(1);
    calc::BigInt<IntT> b(2);
    do_test(a.GenRandom(8), b.GenRandom(4));
    std::cout << std::endl;
    do_test(a.GenRandom(7).ToOpposite(), b.GenRandom(3));
    std::cout << std::endl;
    do_test(a.GenRandom(7), b.GenRandom(3).ToOpposite());
    std::cout << std::endl;
    do_test(a.GenRandom(7).ToOpposite(), b.GenRandom(3).ToOpposite());
    std::cout << std::endl;
    do_more_test(a.GenRandom(3), b.GenRandom(2));
    std::cout << std::endl;
    do_more_test(a.GenRandom(4), b.GenRandom(3));
    return 0;
}
