#include <iostream>
#include "../src/bigint.hpp"
// test &, |, ^, <<, >>, ~
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    calc::BigInt<> a(1);
    calc::BigInt<> b(2);
    std::srand(static_cast<unsigned>(time(nullptr)));
    std::cout << a << " & " << b << " == " << (a & b) << std::endl;
    std::cout << a << " | " << b << " == " << (a | b) << std::endl;
    std::cout << a << " ^ " << b << " == " << (a ^ b) << std::endl;
    std::cout << b << " & " << a << " == " << (b & a) << std::endl;
    std::cout << b << " | " << a << " == " << (b | a) << std::endl;
    std::cout << b << " ^ " << a << " == " << (b ^ a) << std::endl;
    std::cout << "~" << a << " == " << ~a << " <-- True for these bytes"
              << std::endl;
    std::cout << a << " ^ " << ~a << " == " << (a ^ ~a)
              << " <-- Shrinked to one limb" << std::endl;
    for (int i = 0; i <= 32; ++i) {
        // randomly test half, but border is mandatory
        if ((std::rand() & 1) && i % 8) continue;
        std::cout << a << " << " << i << " == " << (a << i) << std::endl;
        std::cout << a << " >> " << i << " == " << (a >> i) << std::endl;
    }
    return 0;
}
