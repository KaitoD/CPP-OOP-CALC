#include <iostream>
#include "../src/bigint.hpp"
// test simple construct, hex I/O, basic operations
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    std::cout << std::hex << std::showbase << std::endl;
    std::cout << "Print zero: " << calc::BigInt<uint32_t>(0) << std::endl;
    // self-copy construction ok, get 0
    calc::BigInt<> a = a;
    // self-move ok, get 0
    calc::BigInt<> b = std::move(b);
    b = b;
    b = std::move(b);
    a.GenRandom(4);
    std::cout << "a = " << a << std::endl;
    b = a;  // copy test
    std::cout << "b = " << b << std::endl;
    b.GenRandom(7);
    std::cout << "b = " << b << std::endl;
    std::cout << "sign: " << a.Sign() << b.Sign() << std::endl;
    return 0;
}
