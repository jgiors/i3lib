///@file
#include "i3prng.h"
#include "i3error.h"
#include "xxh3.h"
#include <exception>

Prng(const Prng &prng, std::vector<byte> &parameterBuffer) {
    XXH3_state_t hasher;
    static_assert(sizeof(hasher) == sizeof(_state));

    I3CHECK(XXH3_128bits_reset(&hasher) == XXH_OK);
    I3CHECK(XXH3_128bits_update(&hasher, parameterBuffer.data, parameterBuffer.size()) == XXH_OK);
    I3CHECK(XXH3_128bits_update(&hasher, _state, sizeof(_state)) == XXH_OK);
    _state = reinterpret_cast<State>(XXH3_128bits_digest(&hasher));
    rand32();
}
