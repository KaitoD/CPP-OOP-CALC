#include <chrono>
#include <iostream>

#include "../src/bigint64.hpp"
using calc::uint128_t;
using BI = calc::BigInt<uint128_t>;
int main() {
    BI a, b, res, and_val(0xff);
    std::random_device ran_dev;
    std::mt19937 ran_eng(ran_dev());
    std::uniform_int_distribution<uint16_t> ran(1, 8192);
    uint64_t tot_len = 0;
    int test_count = 16;
    // eliminate time for random generate
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        tot_len += a.Length();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto rand_dur = end_time - start_time;
    rand_dur /= static_cast<int64_t>(tot_len);
    std::printf("Time per limb for random generation is %.3lfus.\n",
                rand_dur.count() / 1e3);
    std::printf("Note: limb between %hu and %hu\n", ran.min(), ran.max());
    auto duration = (end_time - start_time) - rand_dur * tot_len;

    if (ran.max() <= 8192) {
        tot_len = 0;
        a.Shrink();
        b.Shrink();
        start_time = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < test_count; ++i) {
            a.GenRandom(ran(ran_eng));
            b.GenRandom(ran(ran_eng));
            if (a.Length() > b.Length()) {
                res ^= a.DivD(a, b);
                tot_len += a.Length();
            } else {
                res ^= a.DivD(b, a);
                tot_len += b.Length();
            }
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = (end_time - start_time) - rand_dur * tot_len;
        std::printf("Tested schoolbook division");
        std::printf(" on %d samples. Total length is %llu.\n", test_count,
                    tot_len);
        std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
        std::printf("Execution time per limb*operation is %.3lfus.\n",
                    duration.count() / 1e3 / tot_len);
        std::cout << (res & and_val)
                  << "(prevent optimizing out the whole loop)" << std::endl;
    } else {
        std::cout << "Upper bound too large to test plain case" << std::endl;
    }
    std::cout << std::endl;
    tot_len = 0;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        b.GenRandom(ran(ran_eng));
        if (a.Length() > b.Length()) {
            res ^= a.DivR(a, b);
            tot_len += a.Length();
        } else {
            res ^= a.DivR(b, a);
            tot_len += b.Length();
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested recursive division");
    std::printf(" on %d samples. Total length is %llu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << (res & and_val) << "(prevent optimizing out the whole loop)"
              << std::endl;
    std::cout << std::endl;
    tot_len = 0;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        b.GenRandom(ran(ran_eng));
        if (a.Length() > b.Length()) {
            res ^= a / b;
            tot_len += a.Length();
        } else {
            res ^= b / a;
            tot_len += b.Length();
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested auto-distribute division");
    std::printf(" on %d samples. Total length is %llu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << (res & and_val) << "(prevent optimizing out the whole loop)"
              << std::endl;
    return 0;
}
