#include <chrono>
#include <iostream>
#include <vector>

#include "../src/bigint64.hpp"
using calc::uint128_t;
using BI = calc::BigInt<uint128_t>;
int main() {
    BI a;
    a.GenRandom(3);
    a.Print(16);
    std::puts("");
    auto v = a.Serialize();
    std::cout << std::hex << int(v[0]) << int(v[1]) << std::endl;
    BI b(v.data(), v.size());
    b.Print(16);
    std::puts("");
    return 0;
}
