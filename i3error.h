#ifndef I3ERROR_H
#define I3ERROR_H

#include "i3defs.h"
#include "i3logger.h"
#include <cstdlib>

///Create a software breakpoint in debug build.
inline void i3break() { I3DEBUG_ONLY(__debugbreak()); }

///Abort application.
///Breaks into the code (in debug builds), then exits the program.
[[noreturn]] inline void i3abort() { i3break(); std::abort(); }

///Abort with streaming log message.
///Logs the message, breaks into the code (in debug builds), then exits the program.
///@example I3ABORT(i3logErr << "It's the end of the world!\n");
[[noreturn]] inline void i3abort(i3::Logger &streamingLogMessage) { i3abort(); }

///Check macro which asserts a condition in all builds. On failure, i3abort().
#define I3CHECK(C) if (!(C)) i3abort(i3logErr << "I3CHECK(" << #C << ") failed!\n")

///Check macro which asserts a condition in all builds.
///On failure, print streaming log message and i3abort().
///@example I3CHECK_MSG(x == 0, i3logErr << "Why is x = " << x << "?\n");
#define I3CHECK_MSG(CONDITION, STREAMING_LOG_MESSAGE) do { \
            if (!CONDITION) { \
                i3logErr << "I3CHECK(" << #CONDITION << ") failed: "; \
                i3abort(STREAMING_LOG_MESSAGE); \
            } \
        } while(false)

#endif //I3ERROR_H
