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
   public:
    uint128_t* val_ = nullptr;
    uint64_t len_ = 1;  // actual used length
    uint64_t cap_ = 1;  // capacity, currently must be *a power of 2*
    uint128_t* end_ = nullptr;
    static constexpr uint64_t LIMB = sizeof(uint128_t) << 3;
    static constexpr uint64_t MAX_CAP = uint64_t(1) << 63;

    bool Sign() const;
    BigInt& ToBitInv();
};
}  // namespace calc
