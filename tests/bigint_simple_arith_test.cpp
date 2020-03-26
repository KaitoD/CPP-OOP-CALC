#include <iostream>
#include "../src/bigint.hpp"
// test +, -, ++, --
void do_test(calc::BigInt<>& a, calc::BigInt<>& b) {
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
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    calc::BigInt<> a(1);
    calc::BigInt<> b(2);
    do_test(a.GenRandom(3), b.GenRandom(7));
    std::cout << std::endl;
    do_test(a.GenRandom(3, 16), b.GenRandom(7));
    std::cout << std::endl;
    do_test(a.GenRandom(3), b.GenRandom(7, 16));
    std::cout << std::endl;
    do_test(a.GenRandom(3, 16), b.GenRandom(7, 16));
    return 0;
}
