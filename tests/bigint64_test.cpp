#include <iostream>

#include "../src/bigint64_testoutput.cpp"
using calc::uint128_t;
int main() {
    calc::BigInt<uint128_t> a(0x123456llu);
    a += 0x7654321llu;
    a.Print(16);
    std::puts("");
    return 0;
}
