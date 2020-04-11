#include <chrono>
#include <iostream>

#include "../src/bigint64.hpp"
using calc::uint128_t;
using BI = calc::BigInt<uint128_t>;
int main() {
    BI a, b;
    a.GenRandom(4);
    a.ToNextPrime();
    std::cout << "is " << a << " prime?" << std::endl;
    std::cout << std::boolalpha << a.isProbablePrime() << std::endl;
    std::cout << "This is a prime:" << std::endl;
    std::cout.flush();
    a = BI(
        "1076178293437128762543905423860046918260172323085300723741843414983154"
        "6985795300225003100353833864376314482066276979888950830293120041396754"
        "377320551927531");
    std::cout << "is " << a << " prime?" << std::endl;
    std::cout << std::boolalpha << a.isProbablePrime() << std::endl;
    return 0;
}
