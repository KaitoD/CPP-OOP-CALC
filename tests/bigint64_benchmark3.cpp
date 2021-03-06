#include <chrono>
#include <iostream>

#include "../src/bigint64.hpp"
using calc::uint128_t;
using BI = calc::BigInt<uint128_t>;
int main() {
    BI a, b, x, y, res, and_val(0xff);
    std::random_device ran_dev;
    std::mt19937 ran_eng(ran_dev());
    std::uniform_int_distribution<uint16_t> ran;
    size_t tot_len = 0;
    int test_count = 16;
    uint16_t mask = 0x7f;
    std::cout << std::dec;
    uint64_t x1 = 10000;
    auto start_time = std::chrono::high_resolution_clock::now();
    auto c = calc::Factorial(x1);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = end_time - start_time;
    // std::cout << "math.factorial(" << x1 << ") == " << c << std::endl;
    std::printf("math.log10(math.factorial(%llu)) == %.3lf\n", x1, c.log10());
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
    std::cout << (c & and_val) << "(prevent optimizing out the whole loop)"
              << std::endl;

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
    std::cout << (c & and_val) << "(prevent optimizing out the whole loop)"
              << std::endl;

    test_count = 0;
    tot_len = 8;
    start_time = std::chrono::high_resolution_clock::now();
    std::cout << std::boolalpha;
    a.GenRandom(tot_len, 128);
    std::cout << "Get a prime: " << a.ToNextPrime() << std::endl;
    end_time = std::chrono::high_resolution_clock::now();
    duration = end_time - start_time;
    ++test_count;
    std::printf("Note: length is exactly %lu (%lu bits)\n", tot_len,
                (tot_len << 7));
    std::printf("Tested prime generation.\n");
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    a =
        BI("9168350993493354289721280135289246231112934508397010171811856211345"
           "5455459943863830106860112450170829939921606773965108357833963910419"
           "47467492038417254967");
    start_time = std::chrono::high_resolution_clock::now();
    std::cout << a.isProbablePrime() << std::endl;
    end_time = std::chrono::high_resolution_clock::now();
    duration = end_time - start_time;
    std::printf("Time to verify a prime is %.3lfms.\n", duration.count() / 1e6);
    return 0;
}
