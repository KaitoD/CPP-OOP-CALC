#include "bigint64.hpp"
namespace calc {
BigInt<uint128_t>& BigInt<uint128_t>::DivEq64(int64_t rhs, int64_t* remain) {
    // process as uint64
    uint64_t* it = reinterpret_cast<uint64_t*>(end_);
    bool sign = Sign(), rhs_sign = rhs < 0;
    int64_t mod;
    if (sign) ToOpposite();
    if (rhs < 0) rhs = -rhs;
    asm("xorq %%rdx, %%rdx" : : : "cc");
bi128_dVEy_loop:
    asm goto(R"(
	leaq -8(%0), %0
	movq (%0), %%rax
	divq %1
	movq %%rax, (%0)
	cmpq %0, %2
	jne %l4
	movq %%rdx, %3
)"
             :
             : "r"(it), "g"(rhs), "g"(val_), "m"(mod)
             : "cc", "memory"
             : bi128_dVEy_loop);
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

BigInt<uint128_t>& BigInt<uint128_t>::operator/=(int64_t rhs) {
    return DivEq64(rhs, nullptr);
}
}  // namespace calc
