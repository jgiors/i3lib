#ifndef I3MATH_H
#define I3MATH_H
///@file

namespace i3 {
    namespace math {
        ///Power of 2 ceiling.
        ///@note Values greater than 2^31 return 0.
        ///@note Value of 0 returns 0. If that is important to your code, check for 0.
        ///@see `clp2()` in Hacker's Delight by Henry S Warren Jr
        inline std::uint32_t powerOf2ceil(std::uint32_t u)
        {
            --u;
            u |= u >> 1;
            u |= u >> 2;
            u |= u >> 4;
            u |= u >> 8;
            u |= u >> 16;
            return ++u;
        }

        ///Power of 2 floor.
        ///@note A value of 0 returns 0 (probably unsurprisingly).
        ///@see `flp2()` in Hacker's Delight by Henry S Warren Jr
        inline std::uint32_t powerOf2floor(std::uint32_t u)
        {
            u |= u >> 1;
            u |= u >> 2;
            u |= u >> 4;
            u |= u >> 8;
            u |= u >> 16;
            return u - (u >> 1);
        }

        ///Is value a power of 2?
        ///@note    0 is *not* a power of 2.
        ///@see     Hacker's Delight by Henry S Warren Jr
        inline bool isPowerOf2(std::uint32_t u) { return (u & (u - 1)) == 0 && u != 0; }
    } //namespace math
} //namespace i3

#endif //I3MATH_H
