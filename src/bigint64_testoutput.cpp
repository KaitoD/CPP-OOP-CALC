#include <iomanip>

#include "bigint64.cpp"
#include "bigint64.hpp"
namespace calc {
void BigInt<uint128_t>::Print(int base, int showbase, bool uppercase,
                              std::FILE* f) const {
    if (Sign()) {
        auto tmp_obj = -*this;
        std::fputc('-', f);
        tmp_obj.Print(base, showbase, uppercase, f);
        return;
    } else if (!*this) {
        std::fprintf(f, "0");
        return;
    }
    if (base < 2 || base > 36) base = 16;
    bool suffix_base = showbase == 2;
    char charset[37] = "0123456789abcdefghijklmnopqrstuvwxyz";
    if (uppercase) std::strcpy(charset + 10, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    if (showbase == 1) {
        switch (base) {
            case 2:
                if (uppercase)
                    std::fwrite("0B", 1, 2, f);
                else
                    std::fwrite("0b", 1, 2, f);
                break;
            case 8:
                std::fputc('0', f);
                break;
            case 16:
                if (uppercase)
                    std::fwrite("0X", 1, 2, f);
                else
                    std::fwrite("0x", 1, 2, f);
                break;
            default:
                suffix_base = true;
                break;
        }
    }
    if (base == 16) {
        auto it = end_ - 1;
        if (uppercase) {
            if (!*it) --it;
            if (*(reinterpret_cast<uint64_t*>(it) + 1)) {
                std::fprintf(f, "%llX%016llX",
                             *(reinterpret_cast<uint64_t*>(it) + 1),
                             *reinterpret_cast<uint64_t*>(it));
            } else {
                std::fprintf(f, "%llX", *reinterpret_cast<uint64_t*>(it));
            }
            while (it != val_) {
                --it;
                std::fprintf(f, "%016llX%016llX",
                             *(reinterpret_cast<uint64_t*>(it) + 1),
                             *reinterpret_cast<uint64_t*>(it));
            }
        } else {
            if (!*it) --it;
            if (*(reinterpret_cast<uint64_t*>(it) + 1)) {
                std::fprintf(f, "%llx%016llx",
                             *(reinterpret_cast<uint64_t*>(it) + 1),
                             *reinterpret_cast<uint64_t*>(it));
            } else {
                std::fprintf(f, "%llx", *reinterpret_cast<uint64_t*>(it));
            }
            while (it != val_) {
                --it;
                std::fprintf(f, "%016llx%016llx",
                             *(reinterpret_cast<uint64_t*>(it) + 1),
                             *reinterpret_cast<uint64_t*>(it));
            }
        }
    }
}
std::ostream& operator<<(std::ostream& out, const BigInt<uint128_t>& rhs) {
    // std::fprintf(stdout, "high: %llx, %llx\n",
    // *(reinterpret_cast<uint64_t*>(rhs.end_) - 1),
    // *(reinterpret_cast<uint64_t*>(rhs.end_) - 2));
    if (rhs.Sign()) {
        out.put('-');
        return out << -rhs;
    } else if (!rhs) {
        return out.put('0');
    }
    bool showbase = out.flags() & out.showbase;
    if (out.flags() & out.hex) {
        auto it = rhs.end_ - 1;
        if (!*it) --it;
        if (*(reinterpret_cast<uint64_t*>(it) + 1)) {
            out << *(reinterpret_cast<uint64_t*>(it) + 1);
            out.unsetf(out.showbase);
            out << std::setfill('0') << std::setw(16)
                << *reinterpret_cast<uint64_t*>(it);
        } else {
            out << *reinterpret_cast<uint64_t*>(it);
            out.unsetf(out.showbase);
        }
        while (it != rhs.val_) {
            --it;
            out << std::setfill('0') << std::setw(16)
                << *(reinterpret_cast<uint64_t*>(it) + 1);
            out << std::setfill('0') << std::setw(16)
                << *reinterpret_cast<uint64_t*>(it);
        }
    }
    if (showbase) out.setf(out.showbase);
    return out;
}
}  // namespace calc
