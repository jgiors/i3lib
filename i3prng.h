#ifndef I3PRNG_H
#define I3PRNG_H
///@file

#include <cstdint>
#include <vector>
#include <limits>
#include <type_traits>
#include <algorithm>
#include <bit>

namespace i3 {
    ///32-bit pseudorandom number generator of type xorshift128 (128-bit state).
    ///Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs"
    class Prng {
    public:
        ///Internal PRNG state.
        struct State {
            std::uint32_t a, b, c, d;
        };

    private:
        State _state;

    public:
        ///Create a Prng seeded from default time and entropic sources.
        Prng();

        ///Create a Prng, using the hash of a seed buffer to initialize.
        Prng(const std::vector<std::byte> &seed);

        ///Create a unique, independent Prng by applying a parameterization to a source Prng
        ///(also known as a parameterized "split").
        ///@param prng              The source prng to split from. State is not mutated.
        ///@param parameterBuffer   Data buffer for Prng parameterization. This value is
        ///                         combined with the source Prng._state using a hash function.
        Prng(const Prng &prng, std::vector<std::byte> &parameterBuffer);

        ///Create a unique, independent Prng by applying a parameterization to a source Prng
        ///(also known as a parameterized "split").
        ///@param prng      The source prng to split from. State is not mutated.
        ///@param parameter Prng parameterization value. Combined with the source Prng._state
        ///                 using a hash function, so it must be a trivial plain-data type
        ///                 and should not contain pointers or references.
        template<typename T>
        Prng(const Prng &prng, const T &parameter) {
            static_assert(std::is_trivial<parameter>);
            std::vector<std::byte> buffer;
            std::byte *p = reinterpret_cast<std::byte*>(&parameter);
            buffer.insert(0, p, p + sizeof(parameter));
            return Prng(prng, buffer);
        }

        ///Create a Prng with a given state, e.g. as obtained from the state() function.
        Prng(State state) : _state{state} {}

        ///Get a random unsigned 32-bit value.       
        std::uint32_t random32() {
            std::uint32_t t = _state.d;
            std::uint32_t s = _state.a;
            _state.d = _state.c;
            _state.c = _state.b;
            _state.b = s;
            t ^= t << 11;
            t ^= t >> 8;
            return _state.a = t ^ s ^ (s >> 19);
        }

        ///Typical rand function. Returns value in the range [0, n-1] (or 0 when n == 0).
        std::uint32_t random(std::uint32_t n) {
            return std::uint32_t((n * std::uint64_t{random32()}) >> 32);
        }

        ///Rand function which returns value between 0 and n inclusive.
        std::uint32_t random0toN(std::uint32_t n) {
            return (n < std::numeric_limits<std::uint32_t>::max()) ? random(n+1) : random32();
        }

        ///Pick a random integer in the range [i, j]. For j < i, range is [j, i].
        int32_t randomRange(int32_t i, int32_t j) {
            auto mm = std::minmax(i, j);
            std::uint32_t unsignedDiff = std::bit_cast<std::uint32_t>(mm.second - mm.first);
            return mm.first + std::bit_cast<std::int32_t>(random0toN(unsignedDiff));
        }

        ///Return a random double precision float between 0 and 1 inclusive.
        double randomReal() {
            constexpr double reciprocal = 1.0 / UINT32_MAX; 
            return reciprocal * random32();
        }

        ///Get the internal state.
        State state() const { return _state; }
    };
} //namespace i3

#endif //I3PRNG_H
