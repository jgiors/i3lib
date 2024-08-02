#ifndef I3ERROR_H
#define I3ERROR_H

#include "i3defs.h"
#include "i3logger.h"
#include <cstdlib>
#include <cassert>

#define I3ABORT_MSG(STREAMING_LOG_MESSAGE) do { \
            STREAMING_LOG_MESSAGE; assert(false); std::abort(); \
        } while(false)

///Check macro which asserts a condition in all builds. On failure, i3abort().
#define I3CHECK(C) if (!(C)) I3ABORT_MSG(i3logErr << "I3CHECK(" << #C << ") failed!\n")

///Check macro which asserts a condition in all builds.
///On failure, print streaming log message and i3abort().
///@example I3CHECK_MSG(x == 0, i3logErr << "Why is x = " << x << "?\n");
#define I3CHECK_MSG(CONDITION, STREAMING_LOG_MESSAGE) do { \
            if (!CONDITION) { \
                i3logErr << "I3CHECK(" << #CONDITION << ") failed: "; \
                I3ABORT_MSG(STREAMING_LOG_MESSAGE); \
            } \
        } while(false)

#endif //I3ERROR_H
