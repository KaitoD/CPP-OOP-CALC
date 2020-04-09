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

// Work fine on little-endian machine. On big-endian, reverse the whole vector.
template <>
class BigInt<uint128_t> {
    // data
    uint128_t* val_ = nullptr;
    uint64_t len_ = 2;  // actual used length
    uint64_t cap_ = 4;  // capacity
    uint128_t* end_ = nullptr;
    static constexpr uint64_t LIMB = 128;
    static constexpr uint64_t LOGLIMB = 7;
    static constexpr uint64_t MAX_CAP = uint64_t(1) << 63;

    // random device
    // NOLINTNEXTLINE: c++17 ok
    inline static std::random_device rand_dev_;
    // NOLINTNEXTLINE: c++17 ok
    inline static auto rand_gen_ = static_cast<std::mt19937>(rand_dev_());
    // usage: rand_(rand_gen_)
    // NOLINTNEXTLINE: c++17 ok
    inline static std::uniform_int_distribution<uint64_t> rand_;

   public:
    // bigint64_basic.cpp
    explicit BigInt(int value = 0);
    explicit BigInt(uint64_t value);
    BigInt(const BigInt& rhs);
    BigInt(BigInt&& rhs) noexcept;
    virtual ~BigInt();
    BigInt& operator=(const BigInt& rhs);
    BigInt& operator=(BigInt&& rhs) noexcept;
    BigInt& GenRandom(uint64_t length, uint8_t fixed = 0);
    explicit operator int64_t() const;
    explicit operator bool() const;
    bool Sign() const;
    bool Parity() const;
    const uint128_t* Data() const;
    uint64_t Length() const;
    void ShrinkLen();
    void SetLen(uint64_t new_len, bool preserve_sign);
    // shrink size
    void Shrink();
    double log2() const;
    double log10() const;
    uint64_t TrailingZero() const;

    // bigint64_bit.cpp
    BigInt& ToBitInv();
    BigInt operator~() const;
    BigInt& operator&=(const BigInt& rhs);
    BigInt& operator|=(const BigInt& rhs);
    BigInt& operator^=(const BigInt& rhs);
    BigInt& operator<<=(uint64_t rhs);
    BigInt& operator>>=(uint64_t rhs);

    // bigint64_add.cpp
    BigInt& operator+=(uint64_t rhs);
    BigInt& operator+=(const BigInt& rhs);
    BigInt& BiasedAddEq(const BigInt& rhs, uint64_t bias);
    BigInt& operator++();
    BigInt operator++(int);
    BigInt& operator-=(uint64_t rhs);
    BigInt& operator-=(const BigInt& rhs);
    BigInt& BiasedSubEq(const BigInt& rhs, uint64_t bias);
    BigInt& operator--();
    BigInt operator--(int);
    BigInt& ToOpposite();
    BigInt operator-() const;

    // bigint64_io.cpp
    void Print(int base = 10, int showbase = 1, bool uppercase = false,
               std::FILE* f = stdout) const;
    friend std::ostream& operator<<(std::ostream& out,
                                    const BigInt<uint128_t>& rhs);
    std::string ToString(int base = 10, int showbase = 1,
                         bool uppercase = false) const;

    // bigint64_compare.cpp
#ifdef __cpp_impl_three_way_comparison
    std::weak_ordering operator<=>(const BigInt& rhs) const;
#else
    int Compare(const BigInt& rhs) const;
#endif

    // bigint64_div.cpp
    // note this is not unsigned, to ensure the sign of remain is set correctly
    BigInt& DivEq64(int64_t rhs, int64_t* remain);
    BigInt& operator/=(int64_t rhs);

    // TODO
    template <typename T>
    static void BitRevSort(T* a, size_t n);
    // real transform
    static void RFFT(double* dest, size_t n, bool inv);
    static void RMNT(int64_t* dest, size_t n, bool inv);
    static void RFFTExt(std::complex<long double>* dest, size_t n, bool inv);
    BigInt& RFFTMulEq(const BigInt& rhs);
    BigInt& RMNTMulEq(const BigInt& rhs);
    BigInt& RFFTExtMulEq(const BigInt& rhs);
    BigInt& operator*=(uint64_t rhs);
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
