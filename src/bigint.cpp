#include "bigint.hpp"
// If there are other cpp files, include them here,
// because instanizaiton is only here. Example:
// #include "bigint_mul.cpp"
namespace calc {
// constructors
template <typename IntT>
BigInt<IntT>::BigInt(int value)
    : cap_(sizeof(int) < LIMB ? 1 : sizeof(int) / LIMB),
      len_(0),
      val_(new IntT[cap_]) {
    do {
        val_[len_++] = IntT(value);
        value >>= LIMB;
    } while (value != 0);
    if (len_ < cap_) std::fill(val_ + len_, val_ + cap_, IntT(0));
    if (len_ == 0) len_ = 1;
}
// copy constructor
template <typename IntT>
BigInt<IntT>::BigInt(const BigInt<IntT>& rhs) {
    if (&rhs == this) {
        // self-copy
        cap_ = 1;
        len_ = cap_;
        val_ = new IntT[cap_];
        std::fill(val_, val_ + cap_, IntT(0));
    } else {
        cap_ = rhs.cap_;
        len_ = rhs.len_;
        val_ = new IntT[rhs.cap_];
        std::copy(rhs.val_, rhs.val_ + cap_, val_);
    }
}
// move constructor
template <typename IntT>
BigInt<IntT>::BigInt(BigInt<IntT>&& rhs) noexcept : BigInt() {
    // self-move guaranteed by swap
    std::swap(cap_, rhs.cap_);
    std::swap(len_, rhs.len_);
    std::swap(val_, rhs.val_);
}

// destructor
template <typename IntT>
BigInt<IntT>::~BigInt() {
    delete[] val_;
}

// assignment & type conversion operators
// copy assignment, *only copy the value*
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator=(const BigInt<IntT>& rhs) {
    // check for self-copy assignment
    if (&rhs == this) return *this;
    if (len_ > rhs.len_) {
        std::copy(rhs.val_, rhs.val_ + rhs.len_, val_);
        std::fill(val_ + rhs.len_, val_ + len_, IntT(0));
    } else if (cap_ >= rhs.len_) {
        std::copy(rhs.val_, rhs.val_ + len_, val_);
    } else {
        delete[] val_;
        val_ = new IntT[rhs.cap_];
        std::copy(rhs.val_, rhs.val_ + rhs.cap_, val_);
    }
    cap_ = rhs.cap_;
    len_ = rhs.len_;
    return *this;
}
// move assignment, after move becomes 0, but with whatever capacity
template <typename IntT>
BigInt<IntT>& BigInt<IntT>::operator=(BigInt<IntT>&& rhs) noexcept {
    std::fill(val_, val_ + len_, IntT(0));
    len_ = 1;
    // self-safe guaranteed by swap
    std::swap(cap_, rhs.cap_);
    std::swap(len_, rhs.len_);
    std::swap(val_, rhs.val_);
    return *this;
}

// non-modifying binary operators
// template <typename IntT>
// BigInt<IntT> operator&(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs &= rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator|(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs |= rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator^(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs ^= rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator+(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs += rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator-(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs -= rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator*(BigInt<IntT> lhs, IntT rhs) {
// return lhs *= rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator*(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs *= rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator/(BigInt<IntT> lhs, IntT rhs) {
// return lhs /= rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator/(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs /= rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator%(BigInt<IntT> lhs, IntT rhs) {
// return lhs %= rhs;
// }
// template <typename IntT>
// BigInt<IntT> operator%(BigInt<IntT> lhs, const BigInt<IntT>& rhs) {
// return lhs %= rhs;
// }
// #ifndef __cpp_impl_three_way_comparison
// template <typename IntT>
// bool operator<(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs) {
// return lhs.Compare(rhs) < 0;
// }
// template <typename IntT>
// bool operator>(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs) {
// return lhs.Compare(rhs) > 0;
// }
// template <typename IntT>
// bool operator<=(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs) {
// return lhs.Compare(rhs) <= 0;
// }
// template <typename IntT>
// bool operator>=(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs) {
// return lhs.Compare(rhs) >= 0;
// }
// template <typename IntT>
// bool operator==(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs) {
// return lhs.Compare(rhs) == 0;
// }
// template <typename IntT>
// bool operator!=(const BigInt<IntT>& lhs, const BigInt<IntT>& rhs) {
// return lhs.Compare(rhs) != 0;
// }
// #endif

// explicit instanization
template class BigInt<uint8_t>;
template class BigInt<uint16_t>;
template class BigInt<uint32_t>;

// explicit instanizaiton of operators
// template BigInt<uint8_t> operator&(BigInt<uint8_t> lhs,
// const BigInt<uint8_t>& rhs);
// template BigInt<uint8_t> operator|(BigInt<uint8_t> lhs,
// const BigInt<uint8_t>& rhs);
// template BigInt<uint8_t> operator^(BigInt<uint8_t> lhs,
// const BigInt<uint8_t>& rhs);
// template BigInt<uint8_t> operator+(BigInt<uint8_t> lhs,
// const BigInt<uint8_t>& rhs);
// template BigInt<uint8_t> operator-(BigInt<uint8_t> lhs,
// const BigInt<uint8_t>& rhs);
// template BigInt<uint8_t> operator*(BigInt<uint8_t> lhs, uint8_t rhs);
// template BigInt<uint8_t> operator*(BigInt<uint8_t> lhs,
// const BigInt<uint8_t>& rhs);
// template BigInt<uint8_t> operator/(BigInt<uint8_t> lhs, uint8_t rhs);
// template BigInt<uint8_t> operator/(BigInt<uint8_t> lhs,
// const BigInt<uint8_t>& rhs);
// template BigInt<uint8_t> operator%(BigInt<uint8_t> lhs, uint8_t rhs);
// template BigInt<uint8_t> operator%(BigInt<uint8_t> lhs,
// const BigInt<uint8_t>& rhs);
// template bool operator<(const BigInt<uint8_t>& lhs, const BigInt<uint8_t>&
// rhs); template bool operator>(const BigInt<uint8_t>& lhs, const
// BigInt<uint8_t>& rhs); template bool operator<=(const BigInt<uint8_t>& lhs,
// const BigInt<uint8_t>& rhs);
// template bool operator>=(const BigInt<uint8_t>& lhs,
// const BigInt<uint8_t>& rhs);
// template bool operator==(const BigInt<uint8_t>& lhs,
// const BigInt<uint8_t>& rhs);
// template bool operator!=(const BigInt<uint8_t>& lhs,
// const BigInt<uint8_t>& rhs);
// template BigInt<uint16_t> operator&(BigInt<uint16_t> lhs,
// const BigInt<uint16_t>& rhs);
// template BigInt<uint16_t> operator|(BigInt<uint16_t> lhs,
// const BigInt<uint16_t>& rhs);
// template BigInt<uint16_t> operator^(BigInt<uint16_t> lhs,
// const BigInt<uint16_t>& rhs);
// template BigInt<uint16_t> operator+(BigInt<uint16_t> lhs,
// const BigInt<uint16_t>& rhs);
// template BigInt<uint16_t> operator-(BigInt<uint16_t> lhs,
// const BigInt<uint16_t>& rhs);
// template BigInt<uint16_t> operator*(BigInt<uint16_t> lhs, uint16_t rhs);
// template BigInt<uint16_t> operator*(BigInt<uint16_t> lhs,
// const BigInt<uint16_t>& rhs);
// template BigInt<uint16_t> operator/(BigInt<uint16_t> lhs, uint16_t rhs);
// template BigInt<uint16_t> operator/(BigInt<uint16_t> lhs,
// const BigInt<uint16_t>& rhs);
// template BigInt<uint16_t> operator%(BigInt<uint16_t> lhs, uint16_t rhs);
// template BigInt<uint16_t> operator%(BigInt<uint16_t> lhs,
// const BigInt<uint16_t>& rhs);
// template bool operator<(const BigInt<uint16_t>& lhs,
// const BigInt<uint16_t>& rhs);
// template bool operator>(const BigInt<uint16_t>& lhs,
// const BigInt<uint16_t>& rhs);
// template bool operator<=(const BigInt<uint16_t>& lhs,
// const BigInt<uint16_t>& rhs);
// template bool operator>=(const BigInt<uint16_t>& lhs,
// const BigInt<uint16_t>& rhs);
// template bool operator==(const BigInt<uint16_t>& lhs,
// const BigInt<uint16_t>& rhs);
// template bool operator!=(const BigInt<uint16_t>& lhs,
// const BigInt<uint16_t>& rhs);
// template BigInt<uint32_t> operator&(BigInt<uint32_t> lhs,
// const BigInt<uint32_t>& rhs);
// template BigInt<uint32_t> operator|(BigInt<uint32_t> lhs,
// const BigInt<uint32_t>& rhs);
// template BigInt<uint32_t> operator^(BigInt<uint32_t> lhs,
// const BigInt<uint32_t>& rhs);
// template BigInt<uint32_t> operator+(BigInt<uint32_t> lhs,
// const BigInt<uint32_t>& rhs);
// template BigInt<uint32_t> operator-(BigInt<uint32_t> lhs,
// const BigInt<uint32_t>& rhs);
// template BigInt<uint32_t> operator*(BigInt<uint32_t> lhs, uint32_t rhs);
// template BigInt<uint32_t> operator*(BigInt<uint32_t> lhs,
// const BigInt<uint32_t>& rhs);
// template BigInt<uint32_t> operator/(BigInt<uint32_t> lhs, uint32_t rhs);
// template BigInt<uint32_t> operator/(BigInt<uint32_t> lhs,
// const BigInt<uint32_t>& rhs);
// template BigInt<uint32_t> operator%(BigInt<uint32_t> lhs, uint32_t rhs);
// template BigInt<uint32_t> operator%(BigInt<uint32_t> lhs,
// const BigInt<uint32_t>& rhs);
// template bool operator<(const BigInt<uint32_t>& lhs,
// const BigInt<uint32_t>& rhs);
// template bool operator>(const BigInt<uint32_t>& lhs,
// const BigInt<uint32_t>& rhs);
// template bool operator<=(const BigInt<uint32_t>& lhs,
// const BigInt<uint32_t>& rhs);
// template bool operator>=(const BigInt<uint32_t>& lhs,
// const BigInt<uint32_t>& rhs);
// template bool operator==(const BigInt<uint32_t>& lhs,
// const BigInt<uint32_t>& rhs);
// template bool operator!=(const BigInt<uint32_t>& lhs,
// const BigInt<uint32_t>& rhs);
}  // namespace calc
