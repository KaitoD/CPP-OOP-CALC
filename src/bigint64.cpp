#include "bigint64.hpp"
namespace calc {
BigInt<uint128_t>::BigInt(uint64_t value)
    : val_(reinterpret_cast<uint128_t*>(std::calloc(2, sizeof(uint128_t)))),
      len_(1),
      cap_(2),
      end_(val_ + 1) {
    *val_ = value;
}
BigInt<uint128_t>::~BigInt() { std::free(val_); }
bool BigInt<uint128_t>::Sign() const { return *(end_ - 1) >> (LIMB - 1); }
bool BigInt<uint128_t>::Parity() const { return *val_ & 1; }
BigInt<uint128_t>& BigInt<uint128_t>::ToBitInv() {
    uint128_t* it = val_;
    do {
        *it = ~*it;
        ++it;
    } while (it != end_);
    return *this;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator+=(uint64_t rhs) {
    uint128_t* it = val_;
    *it += rhs;
    ++it;
bi128_pLE_loop:
    asm goto(R"(
    adcq $0, %0
    adcq $0, 8%0
    leaq 16%0, %1
    jc %l2
)"
             :
             : "m"(*it), "r"(it)
             :
             : bi128_pLE_loop);
    *end_ = 0;
    asm goto("jno %l2\ncmpq %0, %1\njne %l2"
             :
             : "r"(end_), "r"(it)
             :
             : bi128_pLE_ret);
    SetLen(len_ + 1, false);
bi128_pLE_ret:
    return *this;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator++() { return *this += 1; }
BigInt<uint128_t> BigInt<uint128_t>::operator++(int) {
    auto obj = *this;
    *this += 1;
    return obj;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator-=(uint64_t rhs) {
    uint128_t* it = val_;
    *it -= rhs;
    ++it;
bi128_pLE_loop:
    asm goto(R"(
    sbbq $0, %0
    sbbq $0, 8%0
    leaq 16%0, %1
    jc %l2
)"
             :
             : "m"(*it), "r"(it)
             :
             : bi128_pLE_loop);
    *end_ = 0;
    asm goto("jno %l2\ncmpq %0, %1\njne %l2"
             :
             : "r"(end_), "r"(it)
             :
             : bi128_pLE_ret);
    SetLen(len_ + 1, false);
bi128_pLE_ret:
    return *this;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator--() { return *this -= 1; }
BigInt<uint128_t> BigInt<uint128_t>::operator--(int) {
    auto obj = *this;
    *this += 1;
    return obj;
}
BigInt<uint128_t>::operator bool() {
    auto it = val_;
    do {
        if (*it) return true;
        ++it;
    } while (it != end_);
    return false;
}
BigInt<uint128_t>& BigInt<uint128_t>::ToOpposite() {
    ToBitInv();
    return *this += 1;
}
BigInt<uint128_t> BigInt<uint128_t>::operator-() {
    auto obj = *this;
    return obj.ToOpposite();
}
void BigInt<uint128_t>::SetLen(uint64_t new_len, bool preserve_sign) {
    if (new_len > len_) {
        if (new_len >= cap_) {
            cap_ <<= 1;
            while (cap_ <= new_len) cap_ <<= 1;
            auto tmp_ptr = std::malloc(sizeof(uint128_t) * cap_);
            std::memcpy(tmp_ptr, val_, len_ * sizeof(uint128_t));
            std::free(val_);
            val_ = reinterpret_cast<uint128_t*>(tmp_ptr);
            tmp_ptr = nullptr;
            std::memset(val_ + new_len, 0, cap_ - new_len);
            if (preserve_sign && Sign())
                std::memset(val_ + len_, -1, new_len - len_);
            else
                std::memset(val_ + len_, 0, new_len - len_);
        } else {
            if (preserve_sign && Sign())
                std::memset(val_ + len_, -1, new_len - len_);
        }
        end_ = val_ + new_len;
        len_ = new_len;
    } else if (new_len < len_) {
        auto it = end_ - 1;
        end_ = val_ + new_len;
        if (preserve_sign) {
            *it <<= 1;
            *it >>= 1;
            if (Sign()) *it |= uint128_t(1) << 127;
        }
        std::memset(end_, 0, len_ - new_len);
        len_ = new_len;
    }
}
}  // namespace calc
