#include <chrono>
#include <iostream>

#include "../src/bigint.hpp"
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    using IntT = uint32_t;
    calc::BigInt<IntT> a(1);
    calc::BigInt<IntT> b(2);
    calc::BigInt<IntT> res;
    std::random_device ran_dev;
    std::mt19937 ran_eng(ran_dev());
    std::uniform_int_distribution<uint16_t> ran;
    size_t tot_len = 0;
    size_t test_count = 512;
    // eliminate time for random generate
    auto start_time = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        tot_len += a.Length();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto rand_dur = end_time - start_time;
    rand_dur /= static_cast<int64_t>(tot_len);
    std::printf("Time per limb for random generation is %.3lfus.\n",
                rand_dur.count() / 1e3);
    tot_len = 0;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        b.GenRandom(ran(ran_eng));
        res ^= a + b;
        res ^= a - b;
        if (a.Length() > b.Length()) {
            res ^= -a;
            tot_len += a.Length();
        } else {
            res ^= -b;
            tot_len += b.Length();
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    auto duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested + -");
    std::printf(" on %lu samples. Total length is %lu.\n", test_count, tot_len);
    std::printf("3 operations per round.\n");
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / 3.0 / tot_len);
    std::cout << (res & calc::BigInt<IntT>(0xff))
              << "(prevent optimizing out the whole loop)" << std::endl;

    tot_len = 0;
    a.Shrink();
    res.Shrink();
    IntT c, tres = 1;
    start_time = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng));
        c = ran(ran_eng);
        res ^= a * c;
        res ^= a / c;
        tres ^= a % c;
        tot_len += a.Length();
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * (tot_len + test_count);
    std::printf("Tested * / %% (big@small)");
    std::printf(" on %lu samples. Total length is %lu.\n", test_count, tot_len);
    std::printf("3 operations per round.\n");
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / 3.0 / tot_len);
    std::cout << (res & calc::BigInt<IntT>(0xff))
              << "(prevent optimizing out the whole loop)" << std::endl;
    return 0;
}
