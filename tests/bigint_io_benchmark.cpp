#include <chrono>
#include <iostream>

#include "../src/bigint.hpp"
int main() {
    calc::BigInt<uint32_t> a(12345);
    auto start_time = std::chrono::high_resolution_clock::now();
    a = calc::Power(a, 12345ul);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = end_time - start_time;
    std::printf("Calculation time: %.3lfms\n", duration.count() / 1e6);
    start_time = std::chrono::high_resolution_clock::now();
    auto str = a.ToString(10);
    // std::cout << std::hex << a << std::endl;
    end_time = std::chrono::high_resolution_clock::now();
    duration = end_time - start_time;
    std::printf("Convert to decimal string time: %.3lfms\n",
                duration.count() / 1e6);
    std::printf("First 100 digits: ");
    std::fwrite(str.data(), 1, 100, stdout);
    std::printf("\nLast 100 digits: ");
    std::fwrite(&str.back() - 99, 1, 100, stdout);
    std::printf("\nlg = %.3lf\n", a.log10());
    // std::cout << std::dec << a << std::endl;
    return 0;
}
