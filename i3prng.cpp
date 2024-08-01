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

    HW_PROFILE_INFOA profile;
    I3CHECK_MSG(GetCurrentHwProfileA(&profile) == 0, i3logErr << "error code " << GetLastError());
    i3log << "     profile: dwDockInfo=" << profile.dwDockInfo << " szHwProfileGuid=" << " szHwProfileGuid=" << profile.szHwProfileGuid << profile.szHwProfileName;
    i3::util::pushData(buffer, &profile, sizeof(profile));

    ///@todo #tentative log buffer contents

    *this = Prng(buffer);
    i3log << "    state = (" << _state.a << ", " << _state.b << ", " << _state.c << ", " << _state.d << ")\n"; 
}

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
