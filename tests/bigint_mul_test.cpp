#include <iostream>

#include "../src/bigint.hpp"
// test different multiplication
using IntT = uint32_t;
void do_test(calc::BigInt<IntT>& a, calc::BigInt<IntT>& b) {
    std::cout << std::hex << std::showbase;
    std::cout << a << " * " << b << " == " << a.PlainMul(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a.FFTMul(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a.NFFTMul(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a.MNTMul(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a.NMNTMul(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a.MulKaratsuba(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a.MulToomCook3(a, b) << std::endl;
}
void do_more_test(calc::BigInt<IntT>& a, calc::BigInt<IntT>& b) {
    std::cout << a << " * " << b << " == " << a.FFTMul(a, b) << std::endl;
    std::cout << a << " * " << b << " == " << a * b << std::endl;
}
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    calc::BigInt<IntT> a(1);
    calc::BigInt<IntT> b(2);
    std::srand(static_cast<unsigned>(time(nullptr)));
    constexpr int len = 15;
    do_test(a.GenRandom(len), b.GenRandom(len));
    std::cout << std::endl;
    do_test(a.GenRandom(len).ToOpposite(), b.GenRandom(len));
    std::cout << std::endl;
    do_test(a.GenRandom(len), b.GenRandom(len).ToOpposite());
    std::cout << std::endl;
    do_test(a.GenRandom(len).ToOpposite(), b.GenRandom(len).ToOpposite());
    std::cout << std::endl;
    do_more_test(a.GenRandom(20), b.GenRandom(50));
    do_more_test(a.GenRandom(20).ToOpposite(), b.GenRandom(50));
    do_more_test(a.GenRandom(20), b.GenRandom(50).ToOpposite());
    do_more_test(a.GenRandom(20).ToOpposite(), b.GenRandom(50).ToOpposite());
    return 0;
}
