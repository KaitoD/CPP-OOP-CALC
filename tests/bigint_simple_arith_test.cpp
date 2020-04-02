#include <iostream>

#include "../src/bigint.hpp"
// test +, -, ++, --
void do_test(calc::BigInt<uint32_t>& a, calc::BigInt<uint32_t>& b) {
    std::cout << std::hex << std::showbase;
    std::cout << a << " + " << b << " == " << (a + b) << std::endl;
    std::cout << a << " - " << b << " == " << (a - b) << std::endl;
    std::cout << b << " + " << a << " == " << (b + a) << std::endl;
    std::cout << b << " - " << a << " == " << (b - a) << std::endl;
    std::cout << a << "++ gets " << (a++) << std::endl;
    std::cout << a << "-- gets " << (a--) << std::endl;
    std::cout << "++" << b << " gets " << (++b) << std::endl;
    std::cout << "--" << b << " gets " << (--b) << std::endl;
    std::cout << "-" << a << " == " << (-a) << std::endl;

    std::cout << std::dec;
    std::cout << a << " + " << b << " == " << (a + b) << std::endl;
    std::cout << a << " - " << b << " == " << (a - b) << std::endl;
    std::cout << b << " + " << a << " == " << (b + a) << std::endl;
    std::cout << b << " - " << a << " == " << (b - a) << std::endl;
    std::cout << a << "++ gets " << (a++) << std::endl;
    std::cout << a << "-- gets " << (a--) << std::endl;
    std::cout << "++" << b << " gets " << (++b) << std::endl;
    std::cout << "--" << b << " gets " << (--b) << std::endl;
    std::cout << "-" << a << " == " << (-a) << std::endl;
}

void do_more_test(calc::BigInt<uint32_t>& a, calc::BigInt<uint32_t>& b,
                  uint32_t c) {
    std::cout << std::hex << std::showbase;
    std::cout << a << " * " << c << " == " << (a * c) << std::endl;
    std::cout << b << " * " << c << " == " << (b * c) << std::endl;
    std::cout << a << " // " << c << " == " << (a / c) << std::endl;
    std::cout << b << " // " << c << " == " << (b / c) << std::endl;
    std::cout << a << " % " << c << " == " << (a % c) << std::endl;
    std::cout << b << " % " << c << " == " << (b % c) << std::endl;

    std::cout << std::dec;
    std::cout << a << " * " << c << " == " << (a * c) << std::endl;
    std::cout << b << " * " << c << " == " << (b * c) << std::endl;
    std::cout << a << " // " << c << " == " << (a / c) << std::endl;
    std::cout << b << " // " << c << " == " << (b / c) << std::endl;
    std::cout << a << " % " << c << " == " << (a % c) << std::endl;
    std::cout << b << " % " << c << " == " << (b % c) << std::endl;
}
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    calc::BigInt<uint32_t> a(1);
    calc::BigInt<uint32_t> b(2);
    std::srand(unsigned(time(nullptr)));
    do_test(a.GenRandom(3), b.GenRandom(7));
    std::cout << std::endl;
    do_test(a.GenRandom(3).ToOpposite(), b.GenRandom(7));
    std::cout << std::endl;
    do_test(a.GenRandom(3), b.GenRandom(7).ToOpposite());
    std::cout << std::endl;
    do_test(a.GenRandom(3).ToOpposite(), b.GenRandom(7).ToOpposite());
    std::cout << std::endl;
    do_more_test(a.GenRandom(3), b.GenRandom(7), uint32_t(std::rand()));
    return 0;
}
