#include "../src/bigint64.hpp"

#include <chrono>
#include <iostream>
using calc::uint128_t;
using BI = calc::BigInt<uint128_t>;
void do_test(const BI& a, const BI& b) {
    std::cout << std::hex << std::showbase;
    std::cout << "print(" << a << " & " << b << " == " << (a & b) << ")"
              << std::endl;
    std::cout << "print(" << a << " | " << b << " == " << (a | b) << ")"
              << std::endl;
    std::cout << "print(" << a << " ^ " << b << " == " << (a ^ b) << ")"
              << std::endl;
    std::cout << "print(" << b << " & " << a << " == " << (b & a) << ")"
              << std::endl;
    std::cout << "print(" << b << " | " << a << " == " << (b | a) << ")"
              << std::endl;
    std::cout << "print(" << b << " ^ " << a << " == " << (b ^ a) << ")"
              << std::endl;
    uint32_t t = rand() & 0xff;
    std::cout << "print(" << a << " << " << t << " == " << (a << t) << ")"
              << std::endl;
    t = rand() & 0xff;
    std::cout << "print(" << b << " << " << t << " == " << (b << t) << ")"
              << std::endl;
    if (!a.Sign()) {
        t = rand() & 0xff;
        std::cout << "print(" << a << " >> " << t << " == " << (a >> t) << ")"
                  << std::endl;
    }
    if (!b.Sign()) {
        t = rand() & 0xff;
        std::cout << "print(" << b << " >> " << t << " == " << (b >> t) << ")"
                  << std::endl;
    }
    std::cout << "print("
              << "~" << a << " == " << ~a << ")" << std::endl;
    std::cout << "print(" << a << " ^ " << ~a << " == " << (a ^ ~a) << ")"
              << std::endl;
    std::cout << "print(" << a << " + " << b << " == " << (a + b) << ")"
              << std::endl;
    std::cout << "print(" << a << " - " << b << " == " << (a - b) << ")"
              << std::endl;
    std::cout << "print(" << b << " + " << a << " == " << (b + a) << ")"
              << std::endl;
    std::cout << "print(" << b << " - " << a << " == " << (b - a) << ")"
              << std::endl;
    std::cout << "print("
              << "-" << a << " == " << (-a) << ")" << std::endl;
    std::cout << "print("
              << "-" << b << " == " << (-b) << ")" << std::endl;
}
int main() {
    BI a(0llu), b;
    constexpr int len1 = 3, len2 = 7, len3 = 2;
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
