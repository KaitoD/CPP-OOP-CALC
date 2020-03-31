#include <chrono>
#include <iostream>
#include <random>
#include "../src/bigint.hpp"
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    calc::BigInt<> a(1);
    calc::BigInt<> b(2);
    calc::BigInt<> res;
    std::random_device ran_dev;
    std::mt19937 ran_eng(ran_dev());
    std::uniform_int_distribution<uint16_t> ran;
    size_t tot_len = 0;
    int test_count = 64;
    uint16_t mask = 0xfff;
    // eliminate time for random generate
    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng) & mask);
        tot_len += a.Length();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto rand_dur = end_time - start_time;
    rand_dur /= static_cast<int64_t>(tot_len);
    std::printf("Time per limb for random generation is %.3lfus.\n",
                rand_dur.count() / 1e3);
    std::printf("Note: limb no longer than %hu\n", mask);
    tot_len = 0;
    // test_count = 16;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng) & mask);
        b.GenRandom(ran(ran_eng) & mask);
        if (a.Length() > b.Length()) {
            res ^= a.DivAlgA(a, b);
            tot_len += a.Length();
        } else {
            res ^= a.DivAlgA(b, a);
            tot_len += b.Length();
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    auto duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested division algorithm A (first order approximation)");
    std::printf(" on %d samples. Total length is %lu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << (res & calc::BigInt<>(0xff))
              << "(prevent optimizing out the whole loop)" << std::endl;
    tot_len = 0;
    // test_count = 64;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng) & mask);
        b.GenRandom(ran(ran_eng) & mask);
        if (a.Length() > b.Length()) {
            res ^= a.DivAlgB(a, b);
            tot_len += a.Length();
        } else {
            res ^= a.DivAlgB(b, a);
            tot_len += b.Length();
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested division algorithm B (second order approximation)");
    std::printf(" on %d samples. Total length is %lu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << (res & calc::BigInt<>(0xff))
              << "(prevent optimizing out the whole loop)" << std::endl;
    // test_count = 64;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng) & mask);
        b.GenRandom(ran(ran_eng) & mask);
        if (a.Length() > b.Length()) {
            res ^= a.DivRecursive(a, b);
            tot_len += a.Length();
        } else {
            res ^= a.DivRecursive(b, a);
            tot_len += b.Length();
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("Tested divide-and-conquer division");
    std::printf(" on %d samples. Total length is %lu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << (res & calc::BigInt<>(0xff))
              << "(prevent optimizing out the whole loop)" << std::endl;
    // test_count = 64;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng) & mask);
        b.GenRandom(ran(ran_eng) & mask);
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
    std::printf("Tested auto distribute division");
    std::printf(" on %d samples. Total length is %lu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << (res & calc::BigInt<>(0xff))
              << "(prevent optimizing out the whole loop)" << std::endl;
    // test_count = 64;
    a.Shrink();
    b.Shrink();
    start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < test_count; ++i) {
        a.GenRandom(ran(ran_eng) & mask);
        b.GenRandom(ran(ran_eng) & mask);
        if (a.Length() > b.Length()) {
            res ^= a % b;
            tot_len += a.Length();
        } else {
            res ^= b % a;
            tot_len += b.Length();
        }
    }
    end_time = std::chrono::high_resolution_clock::now();
    duration = (end_time - start_time) - rand_dur * tot_len;
    std::printf("A test of modulus");
    std::printf(" on %d samples. Total length is %lu.\n", test_count, tot_len);
    std::printf("Total time is %.3lfms.\n", duration.count() / 1e6);
    std::printf("Execution time per limb*operation is %.3lfus.\n",
                duration.count() / 1e3 / tot_len);
    std::cout << (res & calc::BigInt<>(0xff))
              << "(prevent optimizing out the whole loop)" << std::endl;
    return 0;
}
