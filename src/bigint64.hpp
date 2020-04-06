#pragma once
#include <complex>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <random>
#include <utility>
namespace calc {
class CompMp;
template <typename IntT>
class BigInt;

typedef __uint128_t uint128_t;

template <>
class BigInt<uint128_t> {
    // data
    uint128_t* val_ = nullptr;
    uint64_t len_ = 1;  // actual used length
    uint64_t cap_ = 2;  // capacity, currently must be *a power of 2*
    uint128_t* end_ = nullptr;
    static constexpr uint64_t LIMB = sizeof(uint128_t) << 3;
    static constexpr uint64_t MAX_CAP = uint64_t(1) << 63;

   public:
    explicit BigInt(int value = 0);
    explicit BigInt(uint64_t value);
    virtual ~BigInt();
    bool Sign() const;
    bool Parity() const;
    BigInt& ToBitInv();
    BigInt& operator+=(uint64_t rhs);
    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator-=(uint64_t rhs);
    BigInt& operator--();
    BigInt operator--(int);
    void SetLen(uint64_t new_len, bool preserve_sign);
    void Print(int base = 10, int showbase = 1, bool uppercase = false,
               std::FILE* f = stdout);
    explicit operator bool();
    BigInt& ToOpposite();
    BigInt operator-();
};
}  // namespace calc
