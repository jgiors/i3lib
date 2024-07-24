#ifndef I3PRNG_H
#define I3PRNG_H
///@file

#include <cstdint>
#include <vector>
#include <limits>

namespace i3 {
    namespace core {
        ///32-bit pseudorandom number generator of type xorshift128 (128-bit state).
        ///Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs"
        class Prng {
        public:
            using std::uint32_t;

            ///Internal PRNG state.
            struct State {
                uint32_t a, b, c, d;
            };

        private:
            State _state;

        public:
            Prng() = delete;

            ///Create a Prng, using the hash of a seed buffer to initialize.
            Prng(const std::vector<byte> &seed);

            ///Create an independent Prng by hashing the concatenation of the current Prng state
            ///onto a user-supplied parameterBuffer.
            ///@note This is also known as "splitting" a PRNG.
            ///@param prng              The source prng to split from. State is not mutated.
            ///@param parameterBuffer   Data buffer to include in the hash for the split Prng state.
            ///                         Do not pass a large buffer -- it is pass-by-value copied.
            Prng(const Prng &prng, std::vector<byte> parameterBuffer);

            ///Create a Prng with a given state.
            Prng(State state) : _state{state} {}

            ///Get a random unsigned 32-bit value.       
            uint32_t random32() {
                uint32_t t = d;
                uint32_t s = a;
                d = c;
                c = b;
                b = s;
                t ^= t << 11;
                t ^= t >> 8;
                return a = t ^ s ^ (s >> 19);
            }

            ///Typical rand function. Returns value in the range [0, n-1] (or 0 when n == 0).
            uint32_t random(uint32_t n) {
                return uint32_t((n * uint64_t{rand32()}) >> 32);
            }

            ///Rand function which returns value between 0 and n inclusive.
            uint32_t random0toN(uint32_t n) {
                return (n < std::numeric_limits<uint32_t>.max) ? rand(n+1) : rand32();
            }

            ///Pick a random integer in the range [i, j]. For j < i, range is [j, i].
            int32_t randomRange(int32_t i, int32_t j) {
                auto mm std::minmax(i, j);
                uint32_t unsignedDiff = reinterpret_cast<uint32_t>(mm.second - mm.first);
                return mm.first + reinterpret_cast<int32_t>(random0toN(unsignedDiff));
            }

            ///Return a random double precision float between 0 and 1 inclusive.
            double RandomReal() {
                constexpr float reciprocal = 1.0 / UINT32_MAX; 
                return reciprocal * rand32();
            }

            ///Get the internal state.
            State state() const { return _state; }
        };
    } //namespace core
} //namespace i3

#endif //I3PRNG_H
