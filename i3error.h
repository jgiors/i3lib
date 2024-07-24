#ifndef I3ERROR_H
#define I3ERROR_H

#include "i3defs.h"
#include "i3logger.h"
#include <cstdlib>

///Abort with streaming log message.
///Logs the message, breaks into the code (in debug builds), then exits the program.
///@example I3ABORT(i3logError << "It's the end of the world!\n");
#define I3ABORT(LOG_STREAM_MESSAGE) do { (LOG_STREAM_MESSAGE); I3DEBUG_BREAK(); std::abort(); } while (0)

///Check macro which asserts a condition in all builds. On failure, I3ABORT().
#define I3CHECK(C) if (!(C)) I3ABORT(i3logError << "I3CHECK(" << #C << ") failed!\n")

#endif //I3ERROR_H
