#include <chrono>
#include <iostream>
#include "../src/bigint.hpp"
// test different multiplication
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    calc::BigInt<uint32_t> a(1);
    calc::BigInt<uint32_t> b(2);
    calc::BigInt<uint32_t> x, y;
    calc::BigInt<uint32_t> res;
    std::random_device ran_dev;
    std::mt19937 ran_eng(ran_dev());
    std::uniform_int_distribution<uint16_t> ran;
    size_t tot_len = 0;
    int test_count = 16;
    uint16_t mask = 0x1ff;
    std::cout << std::dec;
    uint64_t x1 = 10000;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto c = calc::Factorial<uint16_t>(x1);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = end_time - start_time;
    // std::cout << "math.factorial(" << x1 << ") == " << c << std::endl;
    std::printf("math.log10(math.factorial(%lu)) == %.3lf\n", x1, c.log10());
    std::printf("Time to calculate the exact factorial: %.3lfms\n",
                (end_time - start_time).count() / 1e6);
    tot_len = 0;
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        res ^= calc::GcdBin(a.GenRandom(ran(ran_eng) & mask),
                            b.GenRandom(ran(ran_eng) & mask));
        tot_len += std::max(a.Length(), b.Length());
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = end_time - start_time;
    std::printf("Note: limb no longer than %hu\n", mask);
    std::printf("Tested gcd");
    std::printf(" on %d samples. Total length is %lu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb: %.3lfus\n",
                duration.count() / 1e3 / tot_len);
    std::cout << (c & calc::BigInt<>(0xff))
              << "(prevent optimizing out the whole loop)" << std::endl;

    tot_len = 0;
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        res ^= calc::ExtGcdBin(a.GenRandom(ran(ran_eng) & mask),
                               b.GenRandom(ran(ran_eng) & mask), &x, &y);
        tot_len += std::max(a.Length(), b.Length());
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = end_time - start_time;
    std::printf("Note: limb no longer than %hu\n", mask);
    std::printf("Tested extended gcd");
    std::printf(" on %d samples. Total length is %lu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb: %.3lfus\n",
                duration.count() / 1e3 / tot_len);
    std::cout << (c & calc::BigInt<>(0xff))
              << "(prevent optimizing out the whole loop)" << std::endl;
    return 0;
}
