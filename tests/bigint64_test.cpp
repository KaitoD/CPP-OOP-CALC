#include <iostream>

#include "../src/bigint64_testoutput.cpp"
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
    --a;
    a.Print(16);
    std::puts("");
    ++a;
    a.Print(16);
    std::puts("");
    do_test(a.GenRandom(3), b.GenRandom(7));
    std::cout << std::endl;
    do_test(a.GenRandom(3).ToOpposite(), b.GenRandom(7));
    std::cout << std::endl;
    do_test(a.GenRandom(3), b.GenRandom(7).ToOpposite());
    std::cout << std::endl;
    do_test(a.GenRandom(3).ToOpposite(), b.GenRandom(7).ToOpposite());
    std::cout << std::endl;
    do_test(a.GenRandom(2), b.GenRandom(2));
    std::cout << std::endl;
    do_test(a.GenRandom(2).ToOpposite(), b.GenRandom(2));
    std::cout << std::endl;
    do_test(a.GenRandom(2), b.GenRandom(2).ToOpposite());
    std::cout << std::endl;
    do_test(a.GenRandom(2).ToOpposite(), b.GenRandom(2).ToOpposite());
    return 0;
}
