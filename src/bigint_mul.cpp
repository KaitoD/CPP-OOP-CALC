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
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator*=(const BigInt& rhs) {
    if (len_ < 8 || rhs.len_ < 8)
        return PlainMulEq(rhs);
    else
        return FFTMulEq(rhs);
}
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::PlainMulEq(const BigInt& rhs) {
    BigInt<IntT> result;
    result.AutoExpandSize(len_ + rhs.len_);
    bool sign = Sign() ^ rhs.Sign();
    if (Sign()) ToOpposite();
    if (rhs.Sign()) {
        BigInt<IntT> tmp_obj = -rhs;
        for (size_t i = 0; i < tmp_obj.len_; ++i)
            result += ((*this) * tmp_obj.val_[i]) << (LIMB * i);
    } else {
        for (size_t i = 0; i < rhs.len_; ++i)
            result += ((*this) * rhs.val_[i]) << (LIMB * i);
    }
    if (sign)
        *this = -result;
    else
        *this = result;
    return *this;
}
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::FFTMulEq(const BigInt& rhs) {
    using T = std::complex<double>;
    T* v[2];
    size_t n = 1, i = 0;
    bool sign = Sign() ^ rhs.Sign();
    if (Sign()) ToOpposite();
    while (n < len_ + rhs.len_) n <<= 1;
    v[0] = new T[n];
    v[1] = new T[n];
    for (; i < len_; ++i) v[0][i] = val_[i];
    for (; i < n; ++i) v[0][i] = 0;
    if (rhs.Sign()) {
        BigInt<IntT> tmp_obj = -rhs;
        for (i = 0; i < tmp_obj.len_; ++i) v[1][i] = tmp_obj.val_[i];
    } else {
        for (i = 0; i < rhs.len_; ++i) v[1][i] = rhs.val_[i];
    }
    for (; i < n; ++i) v[1][i] = 0;
    FFT(v[0], n, false);
    FFT(v[1], n, false);
    for (i = 0; i < n; ++i) v[0][i] *= v[1][i];
    FFT(v[0], n, true);
    SetLen(len_ + rhs.len_ + 1, true);
    uint64_t tmp = 0;
    for (i = 0; i < len_ - 1; ++i) {
        tmp =
            (tmp >> LIMB) + static_cast<uint64_t>(std::lround(v[0][i].real()));
        val_[i] = static_cast<IntT>(tmp);
    }
    if (sign) ToOpposite();
    ShrinkLen();
    delete[] v[0];
    delete[] v[1];
    return *this;
}

// non-modifying
template <typename IntT>
BigInt<IntT> BigInt<IntT>::PlainMul(BigInt lhs, const BigInt& rhs) {
    return lhs.PlainMulEq(rhs);
}
template <typename IntT>
BigInt<IntT> BigInt<IntT>::FFTMul(BigInt lhs, const BigInt& rhs) {
    return lhs.FFTMulEq(rhs);
}

// FFT
template <typename IntT>
template <typename T>
void BigInt<IntT>::BitRevSort(T* a, size_t n) {
    // must ensure n is a power of 2
    const uint64_t iv = n >> 1;
    uint64_t j = iv;
    uint64_t k;
    for (uint64_t i = 1; i < n - 1; ++i) {
        if (i < j) std::swap(a[i], a[j]);
        k = iv;
        // do a reversed increment on j
        while (k & j) {
            j ^= k;
            k >>= 1;
        }
        j ^= k;
    }
}
template <typename IntT>
void BigInt<IntT>::FFT(std::complex<double>* dest, size_t n, bool inv) {
    // must ensure n is a power of 2
    using T = std::complex<double>;
    double PI;
    if (inv)
        PI = -std::acos(-1.0);
    else
        PI = std::acos(-1.0);
    size_t i = 0, j = 0;
    size_t step2;
    T omega_n, omega, tmp;
    BitRevSort(dest, n);
    for (size_t step = 2; step <= n; step <<= 1) {
        step2 = step >> 1;
        omega_n = T(std::cos(PI / step2), std::sin(PI / step2));
        omega = T(1.0, 0.0);
        for (j = 0; j < step2; ++j) {
            for (i = 0; i < n; i += step) {
                tmp = omega * dest[i + j + step2];
                dest[i + j + step2] = dest[i + j] - tmp;
                dest[i + j] += tmp;
            }
            omega *= omega_n;
        }
    }
    if (inv)
        for (i = 0; i < n; ++i) dest[i] /= static_cast<double>(n);
}

// non-modifying binary operators
template <typename IntT>
BigInt<IntT> operator*(BigInt<IntT> lhs, IntT rhs) {
    return lhs *= rhs;
}
template <typename IntT>
BigInt<IntT> operator*(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
    return lhs *= rhs;
}
}  // namespace calc
