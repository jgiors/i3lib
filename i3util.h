#ifndef I3UTIL_H
#define I3UTIL_H

#include <cstdint>
#include <vector>
#include <bit>

namespace i3 { namespace util {
    ///Push data ont end of byte vector. Returns the vector reference as a convenience.
    inline std::vector<std::byte>& pushData(std::vector<std::byte> &v, const void *p, size_t size) {
        const std::byte *pB{std::bit_cast<const std::byte*>(p)};
        v.insert(v.end(), pB, pB + size);
        return v;
    }
} } //namespace i3::util

#endif //I3UTIL_H
