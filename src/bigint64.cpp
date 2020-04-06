#include "bigint64.hpp"
namespace calc {
bool BigInt<uint128_t>::Sign() const { return *(end_ - 1) >> (LIMB - 1); }
BigInt<uint128_t>& BigInt<uint128_t>::ToBitInv() {
    uint128_t* it = val_;
    do {
        *it = ~*it;
        ++it;
    } while (it != end_);
    return *this;
}
}  // namespace calc
