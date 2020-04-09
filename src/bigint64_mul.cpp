#include "bigint64.hpp"
namespace calc {

BigInt<uint128_t>& BigInt<uint128_t>::operator*=(uint64_t rhs) {
    bool sign = Sign();
    if (sign) ToOpposite();
    BigInt<uint128_t> carry;
    carry.SetLen(len_ + 1, true);
    uint64_t* it = reinterpret_cast<uint64_t*>(val_);
    uint64_t* rit = reinterpret_cast<uint64_t*>(carry.val_) + 1;
    uint64_t* term = reinterpret_cast<uint64_t*>(end_);
    do {
        asm(R"(
	movq (%0), %%rax
	mulq %2
	movq %%rax, (%0)
	movq %%rdx, (%1)
	movq 8(%0), %%rax
	mulq %2
	movq %%rax, 8(%0)
	movq %%rdx, 8(%1)
)"
            :
            : "r"(it), "r"(rit), "g"(rhs)
            : "cc", "memory", "rax", "rdx");
        it += 2;
        rit += 2;
    } while (it != term);
    *this += carry;
    if (sign) ToOpposite();
    ShrinkLen();
    return *this;
}
}  // namespace calc
