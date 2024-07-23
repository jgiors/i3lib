#ifndef I3PRNG_H
#define I3PRNG_H
///@file

#include <cstdint>
#include <vector>

namespace i3 {
    namespace core {
        ///32-bit random number generator of type xorshift128 (128-bit state).
        ///Algorithm "xor128" from p. 5 of Marsaglia, "Xorshift RNGs"
        class Prng {
            using std::uint32_t;

            uint32_t a, b, c, d;
        public:
            using ByteBuffer = std::vector<uint32_t>;

            Prng() = delete;

            ///Crete a Prng seeded with the hash of a seed buffer.
            Prng(ByteBuffer seed);)

            ///Get a random unsigned 32-bit value.       
            uint32_t rand32();

            ///Typical rand function. Returns a value between 0 and n-1 inclusive (or 0 when n == 0).
            uint32_t rand(uint32_t n);

            ///Rand function which returns value between 0 and n inclusive.
            uint32_t random0toN(uint32_t n);

            ///Pick a random integer in the range [i, j].
            int32_t range(int32_t i, int32_t j);

            ///Return a random double precision float between 0 and 1 inclusive.
            double real();

            ///Splits the random generator and advances the original one step.
            random split();

            ///Splits the random generator, but does not advance the state of the original generator.
            ///@important   Do not split the same generator twice without advancing it,
            ///             otherwise the new generators will be identical and correlated.
            random split_noMutate();



        };
    } //namespace core
} //namespace i3

#endif //I3PRNG_H
