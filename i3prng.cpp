///@file
#include "i3prng.h"
#include "i3logger.h"
#include "i3error.h"
#include "i3util.h"
#include "xxh3.h"
#include <exception>
#include <ctime>
//#include <sysinfoapi.h>

i3::Prng::Prng()
{
    i3log << "Prng::Prng() constructor: Seeding PRNG with time and entropic data:\n";

    std::vector<std::byte> buffer;

    std::time_t epoch = time(NULL);
    i3log << "    epoch = " << epoch << "\n";
    I3CHECK(epoch > 0);
    i3::util::pushData(buffer, &epoch, sizeof(epoch));

    ULONGLONG tickCount64 = GetTickCount64();
    i3log << "    tickCount64 = " << tickCount64 << "\n";
    i3::util::pushData(buffer, &tickCount64, sizeof(tickCount64));

    ///@todo add more seed data for better entropy, like high res timestamp and computer info

    ///@todo #tentative log buffer contents

    *this = Prng(buffer);
    i3log << "    state = (" << _state.a << ", " << _state.b << ", " << _state.c << ", " << _state.d << ")\n"; 
}

/*
static core::random seedRandom(void)
{
    core::profileTimestamp profilerTime;
    time_t t;
    unsigned long long tickCount64;

    slog(LOG_INFO, "Seeding PRNG...");

    ///@todo Library wrapper for ISO timestamp.
    t = time(NULL);
    assert(t > 0);
    char isoTimestamp[32];
    release_assert(strftime(isoTimestamp, sizeof(isoTimestamp), "%F %T", localtime(&t)));
        
    slog(LOG_INFO, "timestamp = %s, epoch = %lld [0x%016llx]", isoTimestamp, t, t);

    profilerTime = core::profile_getMonotonicTimestamp();
    assert(profilerTime > 0);
    slog(LOG_INFO, "High resolution timestamp = 0x%016" PRIx64, profilerTime);

    tickCount64 = GetTickCount64();
    slog(LOG_INFO, "GetTickCount64() = %lld [0x%016llx]", tickCount64, tickCount64);
    assert(tickCount64 > 0);

    core::random seed((uint32_t)t, (uint32_t)(tickCount64 * profilerTime),
        (uint32_t)(tickCount64 * t), (uint32_t)profilerTime);

    ///@todo Fix up this random seed code. I really want it to print the values before hashing.
    slog(LOG_NOTICE, "random seed = { 0x%08x, 0x%08x, 0x%08x, 0x%08x }", seed.a, seed.b, seed.c, seed.d);

    core::random rnd(seed);
    slog(LOG_INFO, "initial random state = { 0x%08x, 0x%08x, 0x%08x, 0x%08x }", rnd.a, rnd.b, rnd.c, rnd.d);

    return rnd;
}
*/

i3::Prng::Prng(const i3::Prng &prng, std::vector<std::byte> &parameterBuffer) {
    XXH3_state_t hasher;

    I3CHECK(XXH3_128bits_reset(&hasher) == XXH_OK);
    I3CHECK(XXH3_128bits_update(&hasher, parameterBuffer.data(), parameterBuffer.size()) == XXH_OK);
    I3CHECK(XXH3_128bits_update(&hasher, &_state, sizeof(_state)) == XXH_OK);
    XXH128_hash_t h = XXH3_128bits_digest(&hasher);
    static_assert(sizeof(h) == sizeof(_state));
    _state = std::bit_cast<State>(h);
    random32();
}
