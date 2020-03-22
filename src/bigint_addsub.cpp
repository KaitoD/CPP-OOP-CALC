#include "bigint.hpp"
namespace calc {
// operator +=,-=,++,--
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator+=(const BigInt& rhs) {
    bool overflow = false;
    auto sign = Sign();
    auto rhs_sign = rhs.Sign();
    auto rhs_empty_limb = rhs_sign ? IntT(-1) : IntT(0);
    if (len_ <= rhs.len_) {
        if (len_ < rhs.len_) SetLen(rhs.len_, false);
        for (size_t i = 0; i < len_; ++i) {
            val_[i] += overflow + rhs.val_[i];
            if (val_[i] < rhs.val_[i] || (overflow && val_[i] <= rhs.val_[i]))
                overflow = true;
            else
                overflow = false;
        }
    } else {
        for (size_t i = 0; i < rhs.len_; ++i) {
            val_[i] += overflow + rhs.val_[i];
            if (val_[i] < rhs.val_[i] || (overflow && val_[i] <= rhs.val_[i]))
                overflow = true;
            else
                overflow = false;
        }
        // implicit alignment of len_
        for (size_t i = rhs.len_; i < len_; ++i) {
            val_[i] += overflow + rhs_empty_limb;
            if (val_[i] < rhs_empty_limb ||
                (overflow && val_[i] <= rhs_empty_limb))
                overflow = true;
            else
                overflow = false;
        }
    }
    // actual overflow is not indicated by variable $(overflow)
    if (sign == rhs_sign && Sign() != sign) {
        SetLen(len_ + 1, false);
        val_[len_ - 1] = rhs_empty_limb;
    } else {
        ShrinkLen();
    }
    return *this;
}
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator-=(const BigInt& rhs) {
    // a-b=~(~a+b)
    ToBitInv();
    *this += rhs;
    ToBitInv();
    return *this;
}
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator++() {
    auto sign = Sign();
    auto max_limb = IntT(-1);
    for (size_t i = 0; i < len_; ++i) {
        if (val_[i] == max_limb) {
            val_[i] = IntT(0);
        } else {
            ++val_[i];
            break;
        }
    }
    if (Sign() && !sign) {
        SetLen(len_ + 1, false);
        val_[len_ - 1] = IntT(0);
    } else {
        ShrinkLen();
    }
    return *this;
}
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator--() {
    auto sign = Sign();
    auto min_limb = IntT(0);
    for (size_t i = len_ - 1; i != size_t(-1); --i) {
        if (val_[i] == min_limb) {
            val_[i] = IntT(-1);
        } else {
            --val_[i];
            break;
        }
    }
    if (!Sign() && sign) {
        SetLen(len_ + 1, false);
        val_[len_ - 1] = IntT(-1);
    } else {
        ShrinkLen();
    }
    return *this;
}
template <typename IntT>
BigInt<IntT> BigInt<IntT>::operator++(int) {
    BigInt<IntT> obj = *this;
    ++*this;
    return obj;
}
template <typename IntT>
BigInt<IntT> BigInt<IntT>::operator--(int) {
    BigInt<IntT> obj = *this;
    --*this;
    return obj;
}
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::ToOpposite() {
    ToBitInv();
    return ++*this;
}
template <typename IntT>
BigInt<IntT> BigInt<IntT>::operator-() const {
    BigInt<IntT> obj = *this;
    return obj.ToOpposite();
}
}  // namespace calc
