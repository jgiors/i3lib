#ifndef I3BYTEBUFFER_H
#define I3BYTEBUFFER_H

#include <vector>
#include <bit>

namespace i3 {
    ///Byte data buffer based on std::vector which adds an append byte data function.
    struct ByteBuffer : public std::vector<std::byte> {
        ///Append data to the end of byte vector. Returns this reference as a convenience.
        ByteBuffer& append(const void *p, size_t size) {
            const std::byte *pB{std::bit_cast<const std::byte*>(p)};
            insert(end(), pB, pB + size);
            return *this;
        }
    };
}

#endif //I3BYTEBUFFER_H
