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
    if (ran.max() <= 1024) {
        tot_len = 0;
        test_count = 16;
        a.Shrink();
        b.Shrink();
        start_time = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < test_count; ++i) {
            a.GenRandom(ran(ran_eng));
            b.GenRandom(ran(ran_eng));
            res ^= a.PlainMul(a, b);
            tot_len += std::max(a.Length(), b.Length());
        }
        end_time = std::chrono::high_resolution_clock::now();
        duration = (end_time - start_time) - rand_dur * tot_len;
        std::printf("Tested plain multiplication");
        std::printf(" on %d samples. Total length is %llu.\n", test_count,
                    tot_len);
        std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
        std::printf("Execution time per limb*operation is %.3lfus.\n",
                    duration.count() / 1e3 / tot_len);
        std::cout << std::dec << (res & and_val)
                  << "(prevent optimizing out the whole loop)" << std::endl;
    } else {
        std::cout << "Upper bound too large to test plain case" << std::endl;
    }
    std::cout << std::endl;
    tot_len = 0;
    test_count = 16;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        b.GenRandom(ran(ran_eng));
        res ^= a.MNTMul(a, b);
        tot_len += std::max(a.Length(), b.Length());
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested MNT multiplication");
    std::printf(" on %d samples. Total length is %llu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << std::dec << (res & and_val)
              << "(prevent optimizing out the whole loop)" << std::endl;
    std::cout << std::endl;
    tot_len = 0;
    test_count = 16;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        b.GenRandom(ran(ran_eng));
        res ^= a.RMNTMul(a, b);
        tot_len += std::max(a.Length(), b.Length());
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested RMNT multiplication");
    std::printf(" on %d samples. Total length is %llu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << std::dec << (res & and_val)
              << "(prevent optimizing out the whole loop)" << std::endl;
    std::cout << std::endl;
    tot_len = 0;
    test_count = 16;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        b.GenRandom(ran(ran_eng));
        res ^= a.RMNTMulUB(a, b);
        tot_len += std::max(a.Length(), b.Length());
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested RMNT unbalanced multiplication");
    std::printf(" on %d samples. Total length is %llu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << std::dec << (res & and_val)
              << "(prevent optimizing out the whole loop)" << std::endl;
    std::cout << std::endl;
    tot_len = 0;
    test_count = 16;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        b.GenRandom(ran(ran_eng));
        res ^= a * b;
        tot_len += std::max(a.Length(), b.Length());
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested auto distribute multiplication");
    std::printf(" on %d samples. Total length is %llu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << std::dec << (res & and_val)
              << "(prevent optimizing out the whole loop)" << std::endl;
    std::cout << std::endl;
    tot_len = 0;
    test_count = 16;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        b.GenRandom(ran(ran_eng));
        res ^= a.Square(a);
        tot_len += std::max(a.Length(), b.Length());
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested square (RMNT)");
    std::printf(" on %d samples. Total length is %llu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << std::dec << (res & and_val)
              << "(prevent optimizing out the whole loop)" << std::endl;
    // std::cout << std::endl;
    return 0;
}
