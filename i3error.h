#ifndef I3ERROR_H
#define I3ERROR_H

#include "i3defs.h"
#include "i3logger.h"
#include <cstdlib>
#include <cassert>

///Abort application. In debug builds, asserts prior to abort.
#define I3ABORT()   do { assert(false); std::abort(); } while(false)

///Abort application with streaming log message. In debug builds, asserts prior to abort.
///@example I3ABORT_MSG(i3logErr << "The horror!\n").
#define I3ABORT_MSG(STREAMING_LOG_MESSAGE)  do { STREAMING_LOG_MESSAGE; I3ABORT(); } while(false)

///Check a condition in all builds (aka release assert). On failure, abort application.
#define I3CHECK(C)  if (!(C)) I3ABORT_MSG(i3logErr << "I3CHECK(" << #C << ") failed!\n")

///Check a condition in all builds (aka release assert).
///On failure, print streaming log message and abort application.
///@example I3CHECK_MSG(x == 0, i3logErr << "Why is x = " << x << "?\n");
#define I3CHECK_MSG(CONDITION, STREAMING_LOG_MESSAGE) do { \
            if (!CONDITION) { \
                i3logErr << "I3CHECK(" << #CONDITION << ") failed: "; \
                I3ABORT_MSG(STREAMING_LOG_MESSAGE); \
            } \
        } while(false)

#endif //I3ERROR_H
