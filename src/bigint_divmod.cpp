#include "bigint.hpp"
namespace calc {
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator/=(IntT rhs) {
    BasicDivEq(rhs);
    return *this;
}
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator%=(IntT rhs) {
    if (rhs == 0) return *this;
    const uint64_t limb_mod = (1l << LIMB) % rhs;
    uint64_t cur_mod = 1;
    uint64_t tot = 0;
    bool sign = Sign();
    if (sign) ToOpposite();
    if (limb_mod == 0) {
        tot = val_[0] % rhs;
    } else {
        for (size_t i = 0; i < len_; ++i) {
            tot = (tot + cur_mod * val_[i]) % rhs;
            cur_mod = cur_mod * limb_mod % rhs;
        }
    }
    if (len_ > 1) std::fill(val_ + 1, val_ + len_, IntT(0));
    len_ = 2;
    val_[0] = IntT(tot);
    if (sign) ToOpposite();
    ShrinkLen();
    return *this;
}
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::BasicDivEq(IntT rhs, IntT* mod) {
    if (rhs == 0 || rhs == 1) return *this;
    uint64_t t = 0;
    bool sign = Sign();
    if (sign) ToOpposite();
    IntT tmp_rhs = rhs, log_rhs = 0;
    while (!(tmp_rhs & 1)) {
        ++log_rhs;
        tmp_rhs >>= 1;
    }

    if (len_ == 1) {
        t = val_[0] % rhs;
        val_[0] /= rhs;
    } else if (tmp_rhs == 1) {
        *this >>= log_rhs;
    } else {
        for (size_t i = len_ - 1; i != size_t(-1); --i) {
            t = ((t % rhs) << LIMB) | val_[i];
            val_[i] = IntT(t / rhs);
        }
    }

    if (sign) ToOpposite();
    ShrinkLen();
    if (mod) {
        if (sign && t)
            *mod = IntT(rhs - t % rhs);
        else
            *mod = IntT(t % rhs);
    }
    return *this;
}

// non-modifying binary operators
template <typename IntT>
BigInt<IntT> operator/(BigInt<IntT> lhs, IntT rhs) {
    return lhs /= rhs;
}
// template <typename IntT>
// BigInt<IntT> operator/(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs /= rhs;
// }
template <typename IntT>
BigInt<IntT> operator%(BigInt<IntT> lhs, IntT rhs) {
    return lhs %= rhs;
}
// template <typename IntT>
// BigInt<IntT> operator%(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs %= rhs;
// }
}  // namespace calc
