#include "bigint64.hpp"
namespace calc {
BigInt<uint128_t>& BigInt<uint128_t>::DivEq64(int64_t rhs, int64_t* remain) {
    // process as uint64
    auto it = end_;
    bool sign = Sign(), rhs_sign = rhs < 0;
    int64_t mod;
    if (sign) ToOpposite();
    if (rhs < 0) rhs = -rhs;
    asm("xorq %%rdx, %%rdx" : : : "rdx");
    do {
        --it;
        asm("movq 8(%0), %%rax\n\t"
            "divq %1\n\t"
            "movq %%rax, 8(%0)\n\t"
            "movq (%0), %%rax\n\t"
            "divq %1\n\t"
            "movq %%rax, (%0)"
            :
            : "r"(it), "g"(rhs)
            : "cc", "memory", "rax", "rdx");
    } while (it > val_);
    asm("movq %%rdx, %0" : "=m"(mod));
    if (sign != rhs_sign) ToOpposite();
    ShrinkLen();
    if (remain) {
        if (sign)
            *remain = -mod;
        else
            *remain = mod;
    }
    return *this;
}
uint64_t BigInt<uint128_t>::DivDCore(BigInt& lhs, const BigInt& rhs,
                                     uint64_t v1, uint64_t v2, uint64_t u1h,
                                     uint64_t u1l, uint64_t u2, uint64_t bias,
                                     bool half_more) {
    uint64_t q, r;
    if (u1h >= v1) {
        q = -1;
    } else {
        asm("movq %2, %%rdx\n\t"
            "movq %3, %%rax\n\t"
            "divq %4\n\t"
            "movq %%rax, %0\n\t"
            "movq %%rdx, %1"

            : "=m"(q), "=m"(r)
            : "m"(u1h), "m"(u1l), "m"(v1)
            : "memory", "rax", "rdx");
        if (uint128_t(q) * v2 > ((uint128_t(r) << 64) | u2)) --q;
    }
    lhs.BiasedSubEq(rhs * q, bias, half_more);
    if (lhs.Sign()) {
        --q;
        lhs.BiasedAddEq(rhs, bias, half_more);
    }
    return q;
}
BigInt<uint128_t> BigInt<uint128_t>::DivD(const BigInt& lhs, const BigInt& rhs,
                                          BigInt* mod) {
    if (!rhs) return lhs;
    if (lhs.Sign()) {
        if (rhs.Sign()) {
            auto tmp = DivDBase(-lhs, -rhs, mod);
            if (mod) mod->ToOpposite();
            return tmp;
        } else {
            auto tmp = DivDBase(-lhs, rhs, mod);
            tmp.ToOpposite();
            if (mod) mod->ToOpposite();
            return tmp;
        }
    } else {
        if (rhs.Sign()) {
            auto tmp = DivDBase(lhs, -rhs, mod);
            tmp.ToOpposite();
            return tmp;
        } else {
            return DivDBase(lhs, rhs, mod);
        }
    }
}
BigInt<uint128_t> BigInt<uint128_t>::DivD(const BigInt& lhs, BigInt&& rhs,
                                          BigInt* mod) {
    if (!rhs) return lhs;
    if (lhs.Sign()) {
        if (rhs.Sign()) {
            rhs.ToOpposite();
            auto tmp = DivDBase(-lhs, rhs, mod);
            if (mod) mod->ToOpposite();
            return tmp;
        } else {
            auto tmp = DivDBase(-lhs, rhs, mod);
            tmp.ToOpposite();
            if (mod) mod->ToOpposite();
            return tmp;
        }
    } else {
        if (rhs.Sign()) {
            rhs.ToOpposite();
            auto tmp = DivDBase(lhs, rhs, mod);
            tmp.ToOpposite();
            return tmp;
        } else {
            return DivDBase(lhs, rhs, mod);
        }
    }
}
BigInt<uint128_t> BigInt<uint128_t>::DivD(BigInt&& lhs, const BigInt& rhs,
                                          BigInt* mod) {
    if (!rhs) return lhs;
    if (lhs.Sign()) {
        lhs.ToOpposite();
        if (rhs.Sign()) {
            auto tmp = DivDBase(std::move(lhs), -rhs, mod);
            if (mod) mod->ToOpposite();
            return tmp;
        } else {
            auto tmp = DivDBase(std::move(lhs), rhs, mod);
            tmp.ToOpposite();
            if (mod) mod->ToOpposite();
            return tmp;
        }
    } else {
        if (rhs.Sign()) {
            auto tmp = DivDBase(std::move(lhs), -rhs, mod);
            tmp.ToOpposite();
            return tmp;
        } else {
            return DivDBase(std::move(lhs), rhs, mod);
        }
    }
}
BigInt<uint128_t> BigInt<uint128_t>::DivD(BigInt&& lhs, BigInt&& rhs,
                                          BigInt* mod) {
    if (!rhs) return lhs;
    if (lhs.Sign()) {
        lhs.ToOpposite();
        if (rhs.Sign()) {
            rhs.ToOpposite();
            auto tmp = DivDBase(std::move(lhs), rhs, mod);
            if (mod) mod->ToOpposite();
            return tmp;
        } else {
            auto tmp = DivDBase(std::move(lhs), rhs, mod);
            tmp.ToOpposite();
            if (mod) mod->ToOpposite();
            return tmp;
        }
    } else {
        if (rhs.Sign()) {
            rhs.ToOpposite();
            auto tmp = DivDBase(std::move(lhs), rhs, mod);
            tmp.ToOpposite();
            return tmp;
        } else {
            return DivDBase(std::move(lhs), rhs, mod);
        }
    }
}
BigInt<uint128_t> BigInt<uint128_t>::DivDBase(BigInt lhs, const BigInt& rhs,
                                              BigInt* mod) {
    // reinterpret as uint64
    if (lhs < rhs) {
        if (mod) *mod = std::move(lhs);
        return BigInt<uint128_t>(0);
    }
    auto testit = rhs.end_ - 1;
    BigInt<uint128_t> rv;
    uint64_t rlen = lhs.len_ - rhs.len_ + 2;
    while (!*testit) {
        --testit;
        ++rlen;
    }
    rv.SetLen(rlen, false);
    uint64_t mov;
    if (*testit >> 64) {
        asm("bsrq 8(%1), %%r9\n\tmovq %%r9, %0"
            : "=m"(mov)
            : "r"(testit)
            : "cc", "memory", "r9");
        mov = 63 - mov;
    } else {
        asm("bsrq (%1), %%r9\n\tmovq %%r9, %0"
            : "=m"(mov)
            : "r"(testit)
            : "cc", "memory", "r9");
        mov = 127 - mov;
    }
    // no real move
    uint128_t tmpv = *testit << mov;
    if (testit > rhs.val_ && mov) tmpv |= *(testit - 1) >> (128 - mov);
    uint64_t v1 = tmpv >> 64, v2 = tmpv;
    uint64_t i = lhs.len_;
    auto rit = rv.val_ + lhs.len_ - (testit - rhs.val_);
    if (!lhs.val_[i - 1]) {
        --i;
        --rit;
    }
    auto rterm = rv.val_;
    if (testit <= rhs.val_) ++rterm;
    uint64_t u1h, u1l, u2;
    uint128_t t1, t2, t3;

    // the loop
    if (mov) {
        while (rit > rterm) {
            --rit;
            --i;

            // high
            t1 = lhs.val_[i + 1];
            t2 = lhs.val_[i];
            t3 = lhs.val_[i - 1];
            t1 = (t1 << mov) | (t2 >> (128 - mov));
            t2 = (t2 << mov) | (t3 >> (128 - mov));
            u1h = t1 >> 64;
            u1l = t1;
            u2 = t2 >> 64;
            *rit =
                DivDCore(lhs, rhs, v1, v2, u1h, u1l, u2, rit - rv.val_, true);
            *rit <<= 64;

            // low
            t1 = lhs.val_[i + 1];
            t2 = lhs.val_[i];
            t3 = lhs.val_[i - 1];
            t1 = (t1 << mov) | (t2 >> (128 - mov));
            t2 = (t2 << mov) | (t3 >> (128 - mov));
            u1h = t1;
            u1l = t2 >> 64;
            u2 = t2;
            *rit |=
                DivDCore(lhs, rhs, v1, v2, u1h, u1l, u2, rit - rv.val_, false);
        }
    } else {
        // mov>=width is UB
        while (rit > rterm) {
            --rit;
            --i;

            // high
            t1 = lhs.val_[i + 1];
            t2 = lhs.val_[i];
            u1h = t1 >> 64;
            u1l = t1;
            u2 = t2 >> 64;
            *rit =
                DivDCore(lhs, rhs, v1, v2, u1h, u1l, u2, rit - rv.val_, true);
            *rit <<= 64;

            // low, must re-extract
            t1 = lhs.val_[i + 1];
            t2 = lhs.val_[i];
            u1h = t1;
            u1l = t2 >> 64;
            u2 = t2;
            *rit |=
                DivDCore(lhs, rhs, v1, v2, u1h, u1l, u2, rit - rv.val_, false);
        }
    }
    if (testit <= rhs.val_) {
        // one more loop without t3
        --rit;
        --i;

        // high
        t1 = lhs.val_[i + 1];
        t2 = lhs.val_[i];
        if (mov) {
            t1 = (t1 << mov) | (t2 >> (128 - mov));
            t2 <<= mov;
        }
        u1h = t1 >> 64;
        u1l = t1;
        u2 = t2 >> 64;
        *rit = DivDCore(lhs, rhs, v1, v2, u1h, u1l, u2, rit - rv.val_, true);
        *rit <<= 64;

        // low
        t1 = lhs.val_[i + 1];
        t2 = lhs.val_[i];
        if (mov) {
            t1 = (t1 << mov) | (t2 >> (128 - mov));
            t2 <<= mov;
        }
        u1h = t1;
        u1l = t2 >> 64;
        u2 = t2;
        *rit |= DivDCore(lhs, rhs, v1, v2, u1h, u1l, u2, rit - rv.val_, false);
    }
    if (mod) {
        *mod = std::move(lhs);
        mod->ShrinkLen();
    }
    rv.ShrinkLen();
    return rv;
}
BigInt<uint128_t>& BigInt<uint128_t>::DivEq(const BigInt& rhs, BigInt* mod) {
    // if (rhs.len_ > 16 && len_ * 2 > rhs.len_ * 3 && len_ < rhs.len_ * 3)
    if (rhs.len_ > 16 && len_ * 2 > rhs.len_ * 3)
        return DivEqR(rhs, mod);
    else if (rhs.len_ < 512 || len_ < rhs.len_ + 512)
        return DivEqD(rhs, mod);
    else
        return DivEqR(rhs, mod);
}
BigInt<uint128_t>& BigInt<uint128_t>::operator%=(int64_t rhs) {
    int64_t mod;
    DivEq64(rhs, &mod);
    SetLen(0, false);
    if (mod < 0) {
        *(val_ + 1) = -1;
        *val_ = -1;
        *val_ <<= 64;
    }
    *val_ |= mod;
    return *this;
}
BigInt<uint128_t> BigInt<uint128_t>::DivRNormal(BigInt lhs, const BigInt& rhs,
                                                BigInt* mod) {
    // normalized: all positive, quotinent not zero,
    // bitlen of rhs divisible by 256
    auto cit = rhs.end_ - 1;
    while (!*cit) --cit;
    // uint64_t t = (cit - rhs.val_ + 1) >> 1;
    uint64_t t = (cit - rhs.val_ + 1);
    t >>= 1;
    uint64_t k = (lhs.len_ + t - 1) / t;
    BigInt<uint128_t> u, v;
    BigInt<uint128_t> rv;
    cit = rhs.val_ + t;
    v.SetLen(rhs.end_ - cit, false);
    std::copy(cit, rhs.end_, v.val_);
    auto term = lhs.end_;
    uint64_t i = k - 1;
    auto it = lhs.val_ + i * t;
    u.SetLen(term - it, false);
    std::copy(it, term, u.val_);
    u = std::move(u) / v;
    lhs.BiasedSubEq(rhs * u, (i - 1) * t, false);
    if (lhs.Sign()) {
        --u;
        lhs.BiasedAddEq(rhs, (i - 1) * t, false);
    }
    if (lhs.Sign()) {
        --u;
        lhs.BiasedAddEq(rhs, (i - 1) * t, false);
    }
    rv.BiasedAddEq(u, (i - 1) * t, false);
    term = lhs.end_;
    for (--i; i > 0; --i) {
        it = lhs.val_ + i * t;
        u.SetLen(term - it + 1, false);
        *(u.end_ - 1) = 0;
        std::copy(it, term, u.val_);
        u.ShrinkLen();
        u = std::move(u) / v;
        lhs.BiasedSubEq(rhs * u, (i - 1) * t, false);
        if (lhs.Sign()) {
            --u;
            lhs.BiasedAddEq(rhs, (i - 1) * t, false);
        }
        if (lhs.Sign()) {
            --u;
            lhs.BiasedAddEq(rhs, (i - 1) * t, false);
        }
        rv.BiasedAddEq(u, (i - 1) * t, false);
        term = lhs.val_ + (i + 1) * t;
    }
    if (mod) *mod = std::move(lhs);
    return rv;
}
BigInt<uint128_t> BigInt<uint128_t>::DivRBase(BigInt lhs, const BigInt& rhs,
                                              BigInt* mod) {
    if (lhs < rhs) {
        if (mod) *mod = std::move(lhs);
        return BigInt<uint128_t>(0);
    }
    auto testit = rhs.end_ - 1;
    while (!*testit) {
        --testit;
    }
    if (testit <= rhs.val_ + 2) {
        return DivDBase(std::move(lhs), rhs, mod);
    } else {
        uint64_t mov;
        if (*testit >> 64) {
            asm("bsrq 8(%1), %%r9\n\tmovq %%r9, %0"
                : "=m"(mov)
                : "r"(testit)
                : "cc", "memory", "r9");
            mov = 63 - mov;
        } else {
            asm("bsrq (%1), %%r9\n\tmovq %%r9, %0"
                : "=m"(mov)
                : "r"(testit)
                : "cc", "memory", "r9");
            mov = 127 - mov;
        }
        if (!((testit - rhs.val_) & 1)) mov += 128;
        // normalized
        if (mov) {
            lhs <<= mov;
            // rvalue of rhs can be optimized
            auto tmp = DivRNormal(std::move(lhs), rhs << mov, mod);
            if (mod) *mod >>= mov;
            return tmp;
        } else {
            auto tmp = DivRNormal(std::move(lhs), rhs, mod);
            return tmp;
        }
    }
}
BigInt<uint128_t> BigInt<uint128_t>::DivR(const BigInt& lhs, const BigInt& rhs,
                                          BigInt* mod) {
    if (!rhs) return lhs;
    if (lhs.Sign()) {
        if (rhs.Sign()) {
            auto tmp = DivRBase(-lhs, -rhs, mod);
            if (mod) mod->ToOpposite();
            return tmp;
        } else {
            auto tmp = DivRBase(-lhs, rhs, mod);
            tmp.ToOpposite();
            if (mod) mod->ToOpposite();
            return tmp;
        }
    } else {
        if (rhs.Sign()) {
            auto tmp = DivRBase(lhs, -rhs, mod);
            tmp.ToOpposite();
            return tmp;
        } else {
            return DivRBase(lhs, rhs, mod);
        }
    }
}
BigInt<uint128_t> BigInt<uint128_t>::DivR(const BigInt& lhs, BigInt&& rhs,
                                          BigInt* mod) {
    if (!rhs) return lhs;
    if (lhs.Sign()) {
        if (rhs.Sign()) {
            rhs.ToOpposite();
            auto tmp = DivRBase(-lhs, rhs, mod);
            if (mod) mod->ToOpposite();
            return tmp;
        } else {
            auto tmp = DivRBase(-lhs, rhs, mod);
            tmp.ToOpposite();
            if (mod) mod->ToOpposite();
            return tmp;
        }
    } else {
        if (rhs.Sign()) {
            rhs.ToOpposite();
            auto tmp = DivRBase(lhs, rhs, mod);
            tmp.ToOpposite();
            return tmp;
        } else {
            return DivRBase(lhs, rhs, mod);
        }
    }
}
BigInt<uint128_t> BigInt<uint128_t>::DivR(BigInt&& lhs, const BigInt& rhs,
                                          BigInt* mod) {
    if (!rhs) return lhs;
    if (lhs.Sign()) {
        lhs.ToOpposite();
        if (rhs.Sign()) {
            auto tmp = DivRBase(std::move(lhs), -rhs, mod);
            if (mod) mod->ToOpposite();
            return tmp;
        } else {
            auto tmp = DivRBase(std::move(lhs), rhs, mod);
            tmp.ToOpposite();
            if (mod) mod->ToOpposite();
            return tmp;
        }
    } else {
        if (rhs.Sign()) {
            auto tmp = DivRBase(std::move(lhs), -rhs, mod);
            tmp.ToOpposite();
            return tmp;
        } else {
            return DivRBase(std::move(lhs), rhs, mod);
        }
    }
}
BigInt<uint128_t> BigInt<uint128_t>::DivR(BigInt&& lhs, BigInt&& rhs,
                                          BigInt* mod) {
    if (!rhs) return lhs;
    if (lhs.Sign()) {
        lhs.ToOpposite();
        if (rhs.Sign()) {
            rhs.ToOpposite();
            auto tmp = DivRBase(std::move(lhs), rhs, mod);
            if (mod) mod->ToOpposite();
            return tmp;
        } else {
            auto tmp = DivRBase(std::move(lhs), rhs, mod);
            tmp.ToOpposite();
            if (mod) mod->ToOpposite();
            return tmp;
        }
    } else {
        if (rhs.Sign()) {
            rhs.ToOpposite();
            auto tmp = DivRBase(std::move(lhs), rhs, mod);
            tmp.ToOpposite();
            return tmp;
        } else {
            return DivRBase(std::move(lhs), rhs, mod);
        }
    }
}
}  // namespace calc
