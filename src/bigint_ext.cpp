#include "bigint.hpp"
namespace calc {
// extended arithmetic
template <typename IntT>
BigInt<IntT> PowMod(const BigInt<IntT>& a, const BigInt<IntT>& p,
                    const BigInt<IntT>& n) {
    BigInt<IntT> tmp, result(1);
    if (p <= BigInt<IntT>(0)) return BigInt<IntT>(1);
    if (a > n)
        tmp = a % n;
    else
        tmp = a;
    IntT mask = 1;
    size_t j, bitlen = p.BitLen();
    for (size_t i = 0; i < p.len_; ++i) {
        for (j = 1, mask = 1; j <= p.LIMB; ++j, mask <<= 1) {
            if (i * p.LIMB + j > bitlen) break;
            if (p.val_[i] & mask) result *= tmp;
            tmp = tmp * tmp;
            if (tmp.len_ > n.len_) tmp %= n;
            if (result.len_ > n.len_) result %= n;
        }
        if (i * p.LIMB + j > bitlen) break;
    }
    return result;
}
template <typename IntT>
BigInt<IntT> GcdBin(BigInt<IntT> a, BigInt<IntT> b) {
    a.ToAbsolute();
    b.ToAbsolute();
    size_t q1 = a.TrailingZero(), q2 = b.TrailingZero();
    int cmp;
    a >>= q1;
    b >>= q2;
    while (true) {
        cmp = a.Compare(b);
        if (cmp < 0) {
            b -= a;
            b >>= b.TrailingZero();
        } else if (cmp > 0) {
            a -= b;
            a >>= a.TrailingZero();
        } else {
            return a <<= std::min(q1, q2);
        }
    }
}
template <typename IntT>
BigInt<IntT> ExtGcdBin(BigInt<IntT> a, BigInt<IntT> b, BigInt<IntT>* x,
                       BigInt<IntT>* y) {
    a.ToAbsolute();
    b.ToAbsolute();
    BigInt<IntT> a0 = a, b0 = b;
    // p*a0+q*b0=a, r*a0+s*b0=b
    BigInt<IntT> p(1), q(0), r(0), s(1);
    size_t t0 = std::min(a.TrailingZero(), b.TrailingZero());
    size_t t1, t2;
    bool swapped = false;
    int cmp;
    a >>= t0;
    b >>= t0;
    if (!a.Parity()) {
        std::swap(a0, b0);
        std::swap(a, b);
        swapped = true;
    }
    if (!b.Parity()) {
        r += b0;
        s -= a0;
        t1 = b.TrailingZero();
        t2 = s.TrailingZero();
        b >>= t1;
        while (t1 > t2) {
            r >>= t2;
            s >>= t2;
            if (r.Sign()) {
                r += b0;
                s -= a0;
            } else {
                r -= b0;
                s += a0;
            }
            t1 -= t2;
            t2 = s.TrailingZero();
        }
        r >>= t1;
        s >>= t1;
    }
    while (true) {
        cmp = a.Compare(b);
        if (cmp < 0) {
            b -= a;
            r -= p;
            s -= q;
            t1 = b.TrailingZero();
            t2 = s.TrailingZero();
            b >>= t1;
            while (t1 > t2) {
                r >>= t2;
                s >>= t2;
                if (r.Sign()) {
                    r += b0;
                    s -= a0;
                } else {
                    r -= b0;
                    s += a0;
                }
                t1 -= t2;
                t2 = s.TrailingZero();
            }
            r >>= t1;
            s >>= t1;
        } else if (cmp > 0) {
            a -= b;
            p -= r;
            q -= s;
            t1 = a.TrailingZero();
            t2 = q.TrailingZero();
            a >>= t1;
            while (t1 > t2) {
                p >>= t2;
                q >>= t2;
                if (p.Sign()) {
                    p += b0;
                    q -= a0;
                } else {
                    p -= b0;
                    q += a0;
                }
                t1 -= t2;
                t2 = q.TrailingZero();
            }
            p >>= t1;
            q >>= t1;
        } else {
            if (swapped) {
                *x = q;
                *y = p;
            } else {
                *x = p;
                *y = q;
            }
            return a <<= t0;
        }
    }
}
}  // namespace calc
