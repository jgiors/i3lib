///@file
#include "i3prng.h"
#include "xxh3.h"
#include <exception>

Prng(const Prng &prng, std::vector<byte> &parameterBuffer) {
    XXH3_state_t hasher;

    ///@todo Some kind of release_assert() or whatnot instead of exceptions (these are hard stop errors).

    if (XXH3_128bits_reset(&hasher) != XXH_OK)
        throw std::runtime_exception("XXH3_128bits_reset");

    if (XXH3_128bits_update(&hasher, parameterBuffer.data, parameterBuffer.size()) != XXH_OK)
        throw std::runtime_exception("XXH3_128bits_update");

    if (XXH3_128bits_update(&hasher, _state, sizeof(_state)) != XXH_OK)
        throw std::runtime_exception("XXH3_128bits_update");

    _state = reinterpret_cast<State>(XXH3_128bits_digest(&hasher));
    rand32();
}
