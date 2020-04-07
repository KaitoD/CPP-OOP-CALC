#include "bigint64.hpp"
namespace calc {
BigInt<uint128_t>::BigInt(int value)
    : val_(new uint128_t[4]), len_(2), cap_(4), end_(val_ + 2) {
    // std::memset(val_, 0, cap_ * sizeof(uint128_t));
    std::fill(val_, val_ + cap_, 0);
    if (value < 0) {
        *val_ = -1;
        *val_ <<= 32;
        *val_ = value;
        *(val_ + 1) = -1;
    } else {
        *val_ = value;
    }
}
BigInt<uint128_t>::BigInt(uint64_t value)
    : val_(new uint128_t[4]), len_(2), cap_(4), end_(val_ + 2) {
    // std::memset(val_, 0, cap_ * sizeof(uint128_t));
    std::fill(val_, val_ + cap_, 0);
    *val_ = value;
}
BigInt<uint128_t>::BigInt(const BigInt<uint128_t>& rhs)
    : val_(new uint128_t[rhs.cap_]),
      len_(rhs.len_),
      cap_(rhs.cap_),
      end_(val_ + len_) {
    // std::memcpy(val_, rhs.val_, rhs.cap_ * sizeof(uint128_t));
    std::copy(rhs.val_, rhs.val_ + rhs.cap_, val_);
}
BigInt<uint128_t>::BigInt(BigInt<uint128_t>&& rhs) noexcept : BigInt() {
    std::swap(val_, rhs.val_);
    std::swap(len_, rhs.len_);
    std::swap(cap_, rhs.cap_);
    std::swap(end_, rhs.end_);
}
BigInt<uint128_t>& BigInt<uint128_t>::operator=(const BigInt<uint128_t>& rhs) {
    if (this == &rhs) return *this;
    if (rhs.len_ < cap_) {
        // std::memcpy(val_, rhs.val_, rhs.len_ * sizeof(uint128_t));
        std::copy(rhs.val_, rhs.val_ + rhs.len_, val_);
        end_ = val_ + rhs.len_;
        if (len_ > rhs.len_)
            // std::memset(end_, 0, (len_ - rhs.len_) * sizeof(uint128_t));
            std::fill(val_ + rhs.len_, val_ + len_, 0);
        len_ = rhs.len_;
    } else {
        delete[] val_;
        val_ = new uint128_t[rhs.cap_];
        // std::memcpy(val_, rhs.val_, rhs.cap_ * sizeof(uint128_t));
        std::copy(rhs.val_, rhs.val_ + rhs.cap_, val_);
        cap_ = rhs.cap_;
        len_ = rhs.len_;
        end_ = val_ + len_;
    }
    return *this;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator=(
    BigInt<uint128_t>&& rhs) noexcept {
    if (this == &rhs) return *this;
    std::swap(val_, rhs.val_);
    std::swap(len_, rhs.len_);
    std::swap(cap_, rhs.cap_);
    std::swap(end_, rhs.end_);
    return *this;
}
BigInt<uint128_t>::~BigInt() { delete[] val_; }
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
BigInt<uint128_t> BigInt<uint128_t>::operator~() const {
    auto obj = *this;
    return obj.ToBitInv();
}
BigInt<uint128_t>& BigInt<uint128_t>::operator+=(uint64_t rhs) {
    // terminal condition ensured by *end == 0
    uint128_t* it = val_ + 1;
    *val_ += rhs;
bi128_pLEy_loop:
    asm goto(R"(
    adcq $0, %0
    adcq $0, 8%0
    leaq 16(%1), %1
    jc %l3
	jno %l4
	cmpq %2, %1
	jne %l4
)"
             :
             : "m"(*it), "r"(it), "r"(end_)
             : "cc", "memory"
             : bi128_pLEy_loop, bi128_pLEy_ret);
    *end_ = 0;
    SetLen(len_ + 1, false);
bi128_pLEy_ret:
    *end_ = 0;
    return *this;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator++() { return *this += 1; }
BigInt<uint128_t> BigInt<uint128_t>::operator++(int) {
    auto obj = *this;
    *this += 1;
    return obj;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator-=(uint64_t rhs) {
    *end_ = -1;  // ensure terminal condition for 0 - 1
    uint128_t* it = val_ + 1;
    *val_ -= rhs;
bi128_mIEy_loop:
    asm goto(R"(
    sbbq $0, %0
    sbbq $0, 8%0
    leaq 16(%1), %1
    jc %l3
	jno %l4
	cmpq %2, %1
	jne %l4
)"
             :
             : "m"(*it), "r"(it), "r"(end_)
             : "cc", "memory"
             : bi128_mIEy_loop, bi128_mIEy_ret);
    *end_ = 0;  // back to default state
    // if signed overflow did occur on highest limb
    SetLen(len_ + 1, false);
    *(end_ - 1) = -1;
bi128_mIEy_ret:
    *end_ = 0;  // back to default state
    return *this;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator--() { return *this -= 1; }
BigInt<uint128_t> BigInt<uint128_t>::operator--(int) {
    auto obj = *this;
    *this -= 1;
    return obj;
}
BigInt<uint128_t>::operator bool() const {
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
BigInt<uint128_t> BigInt<uint128_t>::operator-() const {
    auto obj = *this;
    return obj.ToOpposite();
}
void BigInt<uint128_t>::SetLen(uint64_t new_len, bool preserve_sign) {
    bool sign = Sign();
    // constexpr auto int_size = sizeof(uint128_t);
    if (new_len > len_) {
        if (new_len >= cap_) {
            cap_ <<= 1;
            while (cap_ <= new_len) cap_ <<= 1;
            auto tmp_ptr = new uint128_t[cap_];
            // std::memcpy(tmp_ptr, val_, (end_ - val_) * int_size);
            std::copy(val_, end_, tmp_ptr);
            delete[] val_;
            val_ = tmp_ptr;
            tmp_ptr = nullptr;
            // std::memset(val_ + new_len, 0, (cap_ - new_len) * int_size);
            std::fill(val_ + new_len, val_ + cap_, 0);
            if (preserve_sign && sign)
                // std::memset(val_ + len_, -1, (new_len - len_) * int_size);
                std::fill(val_ + len_, val_ + new_len, -1);
            else
                // std::memset(val_ + len_, 0, (new_len - len_) * int_size);
                std::fill(val_ + len_, val_ + new_len, 0);
        } else {
            if (preserve_sign && sign)
                // std::memset(val_ + len_, -1, (new_len - len_) * int_size);
                std::fill(val_ + len_, val_ + new_len, -1);
        }
        len_ = new_len;
        end_ = val_ + len_;
    } else if (new_len < len_) {
        end_ = val_ + new_len;
        auto it = end_ - 1;
        if (preserve_sign) {
            *it <<= 1;
            *it >>= 1;
            if (sign) *it |= uint128_t(1) << 127;
        }
        // std::memset(end_, 0, (len_ - new_len) * int_size);
        std::fill(val_ + new_len, val_ + len_, 0);
        len_ = new_len;
    }
}
BigInt<uint128_t>& BigInt<uint128_t>::operator&=(const BigInt<uint128_t>& rhs) {
    auto it = val_;
    auto cit = rhs.val_;
    if (len_ == rhs.len_) {
        do {
            *it &= *cit;
            ++it;
            ++cit;
        } while (it != end_);
    } else if (len_ < rhs.len_) {
        bool sign = Sign();
        do {
            *it &= *cit;
            ++it;
            ++cit;
        } while (it != end_);
        if (sign) {
            auto old_len = len_;
            SetLen(rhs.len_, false);
            it = val_ + old_len;
            // std::memcpy(it, cit, (end_ - it) * sizeof(uint128_t));
            std::copy(cit, rhs.end_, it);
        }
    } else {
        do {
            *it &= *cit;
            ++it;
            ++cit;
        } while (cit != rhs.end_);
        if (!rhs.Sign()) {
            SetLen(rhs.len_ + 1, false);
            *(end_ - 1) = 0;
        }
    }
    ShrinkLen();
    return *this;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator|=(const BigInt<uint128_t>& rhs) {
    auto it = val_;
    auto cit = rhs.val_;
    if (len_ == rhs.len_) {
        do {
            *it |= *cit;
            ++it;
            ++cit;
        } while (it != end_);
    } else if (len_ < rhs.len_) {
        bool sign = Sign();
        do {
            *it |= *cit;
            ++it;
            ++cit;
        } while (it != end_);
        if (!sign) {
            auto old_len = len_;
            SetLen(rhs.len_, false);
            it = val_ + old_len;
            // std::memcpy(it, cit, (end_ - it) * sizeof(uint128_t));
            std::copy(cit, rhs.end_, it);
        }
    } else {
        do {
            *it |= *cit;
            ++it;
            ++cit;
        } while (cit != rhs.end_);
        if (rhs.Sign()) {
            SetLen(rhs.len_ + 1, false);
            *(end_ - 1) = -1;
        }
    }
    ShrinkLen();
    return *this;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator^=(const BigInt<uint128_t>& rhs) {
    auto it = val_;
    auto cit = rhs.val_;
    if (len_ == rhs.len_) {
        do {
            *it ^= *cit;
            ++it;
            ++cit;
        } while (it != end_);
    } else if (len_ < rhs.len_) {
        bool sign = Sign();
        do {
            *it ^= *cit;
            ++it;
            ++cit;
        } while (it != end_);
        auto old_len = len_;
        if (sign) {
            SetLen(rhs.len_, false);
            it = val_ + old_len;
            do {
                *it = ~*cit;
                ++it;
                ++cit;
            } while (it != end_);
        } else {
            SetLen(rhs.len_, false);
            it = val_ + old_len;
            // std::memcpy(it, cit, (end_ - it) * sizeof(uint128_t));
            std::copy(cit, rhs.end_, it);
        }
    } else {
        bool rhs_sign = rhs.Sign();
        do {
            *it ^= *cit;
            ++it;
            ++cit;
        } while (cit != rhs.end_);
        if (rhs_sign) {
            do {
                *it = ~*it;
                ++it;
            } while (it != end_);
        }
    }
    ShrinkLen();
    return *this;
}
void BigInt<uint128_t>::ShrinkLen() {
    if (len_ <= 2) return;
    --end_;
    if (*end_ >> (LIMB - 1)) {
        while (!(++*end_) && end_ - val_ >= 2) --end_;
        --*end_;
        if (!(*end_ >> (LIMB - 1))) {
            ++end_;
            --*end_;  // 0 -> -1
        }
        ++end_;
        len_ = end_ - val_;
    } else {
        while (!*end_ && end_ - val_ >= 2) --end_;
        if (*end_ >> (LIMB - 1))
            end_ += 2;
        else
            ++end_;
        len_ = end_ - val_;
    }
}
BigInt<uint128_t>& BigInt<uint128_t>::GenRandom(uint64_t length,
                                                uint8_t fixed) {
    SetLen(length, false);
    auto it = reinterpret_cast<uint64_t*>(val_);
    auto term = reinterpret_cast<uint64_t*>(end_);
    do {
        *it = rand_(rand_gen_);
        ++it;
        *it = rand_(rand_gen_);
        ++it;
    } while (it != term);
    if (fixed) {
        uint128_t mask = 1;
        mask <<= fixed - 1;
        --mask;
        *(end_ - 1) &= mask;
        ++mask;
        *(end_ - 1) |= mask;
    }
    if (Sign()) SetLen(len_ + 1, false);
    return *this;
}
BigInt<uint128_t>::operator int64_t() const {
    int64_t r = static_cast<int64_t>(*val_);
    if (Sign() != bool(r >> 63)) {
        r <<= 1;
        r >>= 1;
        if (Sign()) r |= 0x8000000000000000ll;
    }
    return r;
}
BigInt<uint128_t> operator&(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs) {
    return lhs &= rhs;
}
BigInt<uint128_t> operator|(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs) {
    return lhs |= rhs;
}
BigInt<uint128_t> operator^(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs) {
    return lhs ^= rhs;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator+=(const BigInt& rhs) {
    SetLen(std::max(rhs.len_, len_) + 1, true);
    auto it = val_;
    auto cit = rhs.val_;
    if (rhs.Sign()) {
        *end_ = -1;
        *it += *cit;
        asm("leaq 16(%1), %1\n\tleaq 16(%0), %0" : : "r"(it), "r"(cit));
    bi128_pLER_loop1:
        asm goto(R"(
	movq (%1), %%r8
	movq 8(%1), %%r9
	adcq %%r8, (%0)
	adcq %%r9, 8(%0)
	leaq 16(%1), %1
	leaq 16(%0), %0
	jnc %l3
	cmpq %1, %2
	stc
	jmp %l4
)"
                 :
                 : "r"(it), "r"(cit), "g"(rhs.end_)
                 : "cc", "memory", "r8", "r9"
                 : bi128_pLER_nocarry1, bi128_pLER_endloop1);
    bi128_pLER_nocarry1:
        asm("cmpq %0, %1\n\tclc" : : "g"(cit), "g"(rhs.end_));
    bi128_pLER_endloop1:
        asm goto("jne %l0" : : : : bi128_pLER_loop1);
    // implicit alignment
    bi128_pLER_loop11:
        asm goto(R"(
    adcq $-1, (%0)
    adcq $-1, 8(%0)
    leaq 16(%0), %0
    jnc %l1
)"
                 :
                 : "r"(it)
                 : "cc", "memory"
                 : bi128_pLER_loop11);
    } else {
        *it += *cit;
        asm("leaq 16(%1), %1\n\tleaq 16(%0), %0" : : "r"(it), "r"(cit));
    bi128_pLER_loop2:
        asm goto(R"(
	movq (%1), %%r8
	movq 8(%1), %%r9
	adcq %%r8, (%0)
	adcq %%r9, 8(%0)
	leaq 16(%1), %1
	leaq 16(%0), %0
	jnc %l3
	cmpq %1, %2
	stc
	jmp %l4
)"
                 :
                 : "r"(it), "r"(cit), "g"(rhs.end_)
                 : "cc", "memory", "r8", "r9"
                 : bi128_pLER_nocarry2, bi128_pLER_endloop2);
    bi128_pLER_nocarry2:
        asm("cmpq %0, %1\n\tclc" : : "r"(cit), "g"(rhs.end_));
    bi128_pLER_endloop2:
        asm goto("jne %l0" : : : : bi128_pLER_loop2);
    // implicit alignment
    bi128_pLER_loop21:
        asm goto(R"(
    adcq $0, (%0)
    adcq $0, 8(%0)
    leaq 16(%0), %0
    jc %l1
)"
                 :
                 : "r"(it)
                 : "cc", "memory"
                 : bi128_pLER_loop21);
    }
    *end_ = 0;
    ShrinkLen();
    return *this;
}
BigInt<uint128_t>& BigInt<uint128_t>::operator-=(const BigInt& rhs) {
    SetLen(std::max(rhs.len_, len_) + 1, true);
    auto it = val_;
    auto cit = rhs.val_;
    if (rhs.Sign()) {
        *it -= *cit;
        asm("leaq 16(%1), %1\n\tleaq 16(%0), %0" : : "r"(it), "r"(cit));
    bi128_mIER_loop1:
        asm goto(R"(
	movq (%1), %%r8
	movq 8(%1), %%r9
	sbbq %%r8, (%0)
	sbbq %%r9, 8(%0)
	leaq 16(%1), %1
	leaq 16(%0), %0
	jnc %l3
	cmpq %1, %2
	stc
	jmp %l4
)"
                 :
                 : "r"(it), "r"(cit), "g"(rhs.end_)
                 : "cc", "memory", "r8", "r9"
                 : bi128_mIER_nocarry1, bi128_mIER_endloop1);
    bi128_mIER_nocarry1:
        asm("cmpq %0, %1\n\tclc" : : "r"(cit), "g"(rhs.end_));
    bi128_mIER_endloop1:
        asm goto("jne %l0" : : : : bi128_mIER_loop1);
    // implicit alignment
    bi128_mIER_loop11:
        asm goto(R"(
    sbbq $-1, (%0)
    sbbq $-1, 8(%0)
    leaq 16(%0), %0
    jnc %l1
)"
                 :
                 : "r"(it)
                 : "cc", "memory"
                 : bi128_mIER_loop11);
    } else {
        *end_ = -1;
        *it -= *cit;
        asm("leaq 16(%1), %1\n\tleaq 16(%0), %0" : : "r"(it), "r"(cit));
    bi128_mIER_loop2:
        asm goto(R"(
	movq (%1), %%r8
	movq 8(%1), %%r9
	sbbq %%r8, (%0)
	sbbq %%r9, 8(%0)
	leaq 16(%1), %1
	leaq 16(%0), %0
	jnc %l3
	cmpq %1, %2
	stc
	jmp %l4
)"
                 :
                 : "r"(it), "r"(cit), "g"(rhs.end_)
                 : "cc", "memory", "r8", "r9"
                 : bi128_mIER_nocarry2, bi128_mIER_endloop2);
    bi128_mIER_nocarry2:
        asm("cmpq %0, %1\n\tclc" : : "r"(cit), "g"(rhs.end_));
    bi128_mIER_endloop2:
        asm goto("jne %l0" : : : : bi128_mIER_loop2);
    // implicit alignment
    bi128_mIER_loop21:
        asm goto(R"(
    sbbq $0, (%0)
    sbbq $0, 8(%0)
    leaq 16(%0), %0
    jc %l1
)"
                 :
                 : "r"(it)
                 : "cc", "memory"
                 : bi128_mIER_loop21);
    }
    *end_ = 0;
    ShrinkLen();
    return *this;
}
BigInt<uint128_t> operator+(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs) {
    lhs += rhs;
    return lhs;
}
BigInt<uint128_t> operator-(BigInt<uint128_t> lhs,
                            const BigInt<uint128_t>& rhs) {
    return lhs -= rhs;
}
}  // namespace calc
