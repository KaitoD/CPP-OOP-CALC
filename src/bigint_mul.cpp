#include "bigint.hpp"
namespace calc {
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator*=(IntT rhs) {
    if (rhs == 0) {
        std::fill(val_, val_ + len_, IntT(0));
        len_ = 1;
        return *this;
    } else if (rhs == 1) {
        return *this;
    }
    uint64_t t = 0;
    bool sign = Sign();
    if (sign) ToOpposite();

    IntT tmp_rhs = rhs, log_rhs = 0;
    while (!(tmp_rhs & 1)) {
        ++log_rhs;
        tmp_rhs >>= 1;
    }

    if (tmp_rhs == 1) {
        *this <<= log_rhs;
    } else {
        for (size_t i = 0; i < len_; ++i) {
            t = (t >> LIMB) + uint64_t(rhs) * val_[i];
            val_[i] = static_cast<IntT>(t);
        }
        t >>= LIMB;
        if (t) {
            SetLen(len_ + 1, true);
            val_[len_ - 1] = static_cast<IntT>(t);
        }
    }

    if (sign) ToOpposite();
    return *this;
}

// non-modifying binary operators
template <typename IntT>
BigInt<IntT> operator*(BigInt<IntT> lhs, IntT rhs) {
    return lhs *= rhs;
}
// template <typename IntT>
// BigInt<IntT> operator*(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs *= rhs;
// }
}  // namespace calc
