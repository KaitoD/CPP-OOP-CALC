#include <iostream>
#include "../src/bigint.hpp"
// test different multiplication
void do_test(calc::BigInt<>& a, calc::BigInt<>& b) {
    calc::BigInt<> c;
    std::cout << std::hex << std::showbase;
    std::cout << a << " == " << b << " * " << a.DivAlgA(a, b, &c);
    std::cout << " + " << c << std::endl;
    std::cout << a << " == " << b << " * " << a.DivAlgB(a, b, &c);
    std::cout << " + " << c << std::endl;

    std::cout << std::dec;
    std::cout << a << " == " << b << " * " << a.DivAlgA(a, b, &c);
    std::cout << " + " << c << std::endl;
    std::cout << a << " == " << b << " * " << a.DivAlgB(a, b, &c);
    std::cout << " + " << c << std::endl;
}
void do_more_test(calc::BigInt<>& a, calc::BigInt<>& b) {
    calc::BigInt<> c;
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
    calc::BigInt<> a(1);
    calc::BigInt<> b(2);
    do_test(a.GenRandom(7), b.GenRandom(3));
    std::cout << std::endl;
    do_test(a.GenRandom(7, 16), b.GenRandom(3));
    std::cout << std::endl;
    do_test(a.GenRandom(7), b.GenRandom(3, 16));
    std::cout << std::endl;
    do_test(a.GenRandom(7, 16), b.GenRandom(3, 16));
    std::cout << std::endl;
    do_more_test(a.GenRandom(3), b.GenRandom(2));
    std::cout << std::endl;
    do_more_test(a.GenRandom(4), b.GenRandom(3));
    return 0;
}
