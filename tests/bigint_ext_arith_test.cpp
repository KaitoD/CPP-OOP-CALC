#include <iostream>
#include "../src/bigint.hpp"
// test different multiplication
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    calc::BigInt<> a(1);
    calc::BigInt<> b(2);
    calc::BigInt<> x, y;
    std::srand(static_cast<unsigned>(time(nullptr)));
    std::cout << std::dec;
    uint64_t x1 = 100;
    auto c = calc::Factorial<uint16_t>(x1);
    std::cout << "math.factorial(" << x1 << ") == " << c << std::endl;
    size_t test_count = 24;
    for (size_t i = 0; i < test_count; ++i) {
        c = calc::ExtGcdBin(a.GenRandom(5), b.GenRandom(7), &x, &y);
        std::cout << c << " == math.gcd(" << a << ", " << b << ")" << std::endl;
        std::cout << c << " == " << a << " * " << x << " + " << b << " * " << y
                  << std::endl;
    }
    std::cout << std::endl;
    test_count = 24;
    do {
        a.GenRandom(32);
        if (!a.Parity()) ++a;
    } while (!a.isProbablePrime());
    std::cout << "is " << a << " prime?" << std::endl;
    std::cout << std::boolalpha << true << std::endl;
    std::cout << "This is a prime:" << std::endl;
    a = calc::BigInt<>(
        "1076178293437128762543905423860046918260172323085300723741843414983154"
        "6985795300225003100353833864376314482066276979888950830293120041396754"
        "377320551927531");
    std::cout << "is " << a << " prime?" << std::endl;
    std::cout << std::boolalpha << a.isProbablePrime() << std::endl;
    return 0;
}
