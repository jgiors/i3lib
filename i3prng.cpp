///@file
#include "i3prng.h"
#include "i3logger.h"
#include "i3error.h"
#include "xxh3.h"
#include <exception>

i3::core::Prng::Prng(const i3::core::Prng &prng, std::vector<std::byte> &parameterBuffer) {
    XXH3_state_t hasher;

    I3CHECK(XXH3_128bits_reset(&hasher) == XXH_OK);
    I3CHECK(XXH3_128bits_update(&hasher, parameterBuffer.data(), parameterBuffer.size()) == XXH_OK);
    I3CHECK(XXH3_128bits_update(&hasher, &_state, sizeof(_state)) == XXH_OK);
    XXH128_hash_t h = XXH3_128bits_digest(&hasher);
    static_assert(sizeof(h) == sizeof(_state));
    _state = std::bit_cast<State>(h);
    random32();
}
