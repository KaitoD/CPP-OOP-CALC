#include <iostream>
#include "../src/bigint.hpp"
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    // self-copy construction ok, get 0
	calc::BigInt<> a = a;
    // self-move ok, get 0
    calc::BigInt<> b = std::move(b);
    b = b;
    b = std::move(b);
    return 0;
}
