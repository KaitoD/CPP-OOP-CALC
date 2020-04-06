#include "bigint64.cpp"
#include "bigint64.hpp"
namespace calc {
void BigInt<uint128_t>::Print(int base, int showbase, bool uppercase,
                              std::FILE* f) {
    if (Sign()) {
        auto tmp_obj = -*this;
        std::fputc('-', f);
        tmp_obj.Print(base, uppercase, showbase, f);
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
            std::printf("%08llX%08llX", *(reinterpret_cast<uint64_t*>(it) + 1),
                        *reinterpret_cast<uint64_t*>(it));
            while (it != val_) {
                --it;
                std::printf("%08llX%08llX",
                            *(reinterpret_cast<uint64_t*>(it) + 1),
                            *reinterpret_cast<uint64_t*>(it));
            }
        } else {
            std::printf("%08llx%08llx", *(reinterpret_cast<uint64_t*>(it) + 1),
                        *reinterpret_cast<uint64_t*>(it));
            while (it != val_) {
                --it;
                std::printf("%08llx%08llx",
                            *(reinterpret_cast<uint64_t*>(it) + 1),
                            *reinterpret_cast<uint64_t*>(it));
            }
        }
    }
}
}  // namespace calc
