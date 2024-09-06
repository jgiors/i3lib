///@file
#include "precomp.h"
#include "i3byteBuffer.h"

i3::Prng::Prng()
{
    i3log << "Prng::Prng() constructor: Seeding PRNG with time and entropic data:\n";

    i3::ByteBuffer buffer;

    std::time_t epoch = time(NULL);
    i3log << "    epoch = " << epoch << "\n";
    I3CHECK(epoch > 0);
    buffer.append(&epoch, sizeof(epoch));

    ULONGLONG tickCount64 = GetTickCount64();
    i3log << "    tickCount64 = " << tickCount64 << "\n";
    buffer.append(&tickCount64, sizeof(tickCount64));

    HW_PROFILE_INFOA profile;
    I3CHECK_MSG(GetCurrentHwProfileA(&profile) != 0, i3logErr << "error code " << GetLastError() << "\n");
    i3log << "    profile:"
        << "\n        dwDockInfo = " << profile.dwDockInfo
        << "\n        szHwProfileGuid = " << profile.szHwProfileGuid
        << "\n        szHwProfileGuid = " << profile.szHwProfileName << "\n";
    buffer.append(&profile.dwDockInfo, sizeof(profile.dwDockInfo));
    buffer.append(profile.szHwProfileGuid, strlen(profile.szHwProfileGuid));
    buffer.append(profile.szHwProfileName, strlen(profile.szHwProfileName));

    std::string hex;
    for (std::byte b : buffer)
         hex += std::format("{:02x}", uint8_t(b));
    i3log << "    bufferHex = " << hex << "\n";

    *this = Prng(buffer);
    i3log << "    Prng._state = (" << _state.a << ", " << _state.b << ", " << _state.c << ", " << _state.d << ")\n"; 
}

i3::Prng::Prng(const std::vector<std::byte> &seed)
{
    XXH3_state_t hasher;
    I3CHECK(XXH3_128bits_reset(&hasher) == XXH_OK);
    I3CHECK(XXH3_128bits_update(&hasher, seed.data(), seed.size()) == XXH_OK);
    XXH128_hash_t h = XXH3_128bits_digest(&hasher);
    _state = std::bit_cast<State>(h);
    random32();
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
