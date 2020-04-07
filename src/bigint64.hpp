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
    uint64_t len_ = 2;  // actual used length
    uint64_t cap_ = 4;  // capacity
    uint128_t* end_ = nullptr;
    static constexpr uint64_t LIMB = sizeof(uint128_t) << 3;
    static constexpr uint64_t MAX_CAP = uint64_t(1) << 63;

    // random device
    // NOLINTNEXTLINE: c++17 ok
    inline static std::random_device rand_dev_;
    // NOLINTNEXTLINE: c++17 ok
    inline static auto rand_gen_ = static_cast<std::mt19937>(rand_dev_());
    // usage: rand_(rand_gen_)
    // NOLINTNEXTLINE: c++17 ok
    inline static std::uniform_int_distribution<uint64_t> rand_;

    void ShrinkLen();
    void SetLen(uint64_t new_len, bool preserve_sign);

   public:
    explicit BigInt(int value = 0);
    explicit BigInt(uint64_t value);
    BigInt(const BigInt& rhs);
    BigInt(BigInt&& rhs) noexcept;
    virtual ~BigInt();
    BigInt& operator=(const BigInt& rhs);
    BigInt& operator=(BigInt&& rhs) noexcept;
    bool Sign() const;
    bool Parity() const;
    BigInt& ToBitInv();
    BigInt operator~() const;
    BigInt& operator+=(uint64_t rhs);
    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator-=(uint64_t rhs);
    BigInt& operator--();
    BigInt operator--(int);
    void Print(int base = 10, int showbase = 1, bool uppercase = false,
               std::FILE* f = stdout) const;
    explicit operator bool() const;
    BigInt& ToOpposite();
    BigInt operator-() const;
    BigInt& operator&=(const BigInt& rhs);
    BigInt& operator|=(const BigInt& rhs);
    BigInt& operator^=(const BigInt& rhs);
    BigInt& GenRandom(uint64_t length, uint8_t fixed = 0);
    explicit operator int64_t() const;
    BigInt& operator+=(const BigInt& rhs);
    BigInt& operator-=(const BigInt& rhs);
    friend std::ostream& operator<<(std::ostream& out,
                                    const BigInt<uint128_t>& rhs);
    const uint128_t* Data() const;
    uint64_t Length() const;
};
BigInt<uint128_t> operator&(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs);
BigInt<uint128_t> operator|(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs);
BigInt<uint128_t> operator^(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs);
BigInt<uint128_t> operator+(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs);
BigInt<uint128_t> operator-(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs);
std::ostream& operator<<(std::ostream& out, const BigInt<uint128_t>& rhs);
}  // namespace calc
