#include <iostream>
#include "../src/bigint.hpp"
// test different multiplication
void do_test(calc::BigInt<>& a, calc::BigInt<>& b) {
    std::cout << std::hex << std::showbase;
    std::cout << a << " * " << b << " == " << a.PlainMul(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a.FFTMul(a, b) << std::endl;

    std::cout << std::dec;
    std::cout << a << " * " << b << " == " << a.PlainMul(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a.FFTMul(a, b) << std::endl;
}
void do_more_test(calc::BigInt<>& a, calc::BigInt<>& b) {
    std::cout << a << " * " << b << " == " << a.FFTMul(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a * b << std::endl;
}
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    calc::BigInt<> a(1);
    calc::BigInt<> b(2);
    std::srand(static_cast<unsigned>(time(nullptr)));
    do_test(a.GenRandom(3), b.GenRandom(7));
    std::cout << std::endl;
    do_test(a.GenRandom(3).ToOpposite(), b.GenRandom(7));
    std::cout << std::endl;
    do_test(a.GenRandom(3), b.GenRandom(7).ToOpposite());
    std::cout << std::endl;
    do_test(a.GenRandom(3).ToOpposite(), b.GenRandom(7).ToOpposite());
    std::cout << std::endl;
    do_more_test(a.GenRandom(20), b.GenRandom(50));
    do_more_test(a.GenRandom(20).ToOpposite(), b.GenRandom(50));
    do_more_test(a.GenRandom(20), b.GenRandom(50).ToOpposite());
    do_more_test(a.GenRandom(20).ToOpposite(), b.GenRandom(50).ToOpposite());
    return 0;
}
