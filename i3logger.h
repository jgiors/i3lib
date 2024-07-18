#ifndef I3LOGGER_H
#define I3LOGGER_H
///@file

#include <Windows.h>
#include <memory>
#include <initializer_list>
#include <vector>
#include <tuple>
#include <string>
#include <sstream>
#include "i3lib.h"

///@todo Make filename function which removes the path info (can it be done as a constexpr?).

///Filename and line number format.
///@note The __FILE__ part is not concatenated with line number as a size optimization.
///Letting __FILE__ stand alone means the linker can consoldiate all instance into one string.
///That would not be possible wiht line numbers appended to each instance.
#define I3LOG_FILE_AND_LINE     __FILE__  << "(" I3STRINGIZE(__LINE__) "): "

///Default informational log stream.
#define i3log           i3::core::Logger::i3log_instance << I3LOG_FILE_AND_LINE " "
///Warning log stream.
#define i3logWarning    i3::core::Logger::i3logWarning_instance << I3LOG_FILE_AND_LINE "WARNING! "
///Error log stream.
#define i3logError      i3::core::Logger::i3logError_instance << I3LOG_FILE_AND_LINE "!!! ERROR !!! "

///Debug log stream. Compile out in debug (assuming compiler properly optimizes it out).
#if DEBUG
#   define i3logDebug   i3::core::Logger::i3logDebug_instance << I3LOG_FILE_AND_LINE "[DEBUG] "
#else
#   define i3logDebug   if (false) cerr
#endif

namespace i3 {
    namespace core {
        ///Primary logger class. Use global instances for logging.
        class Logger {
            ///Group of streams. Streaming to this object forwards to each referenced stream.
            ///Also supports debug output stream (on Windows, this is routed to the debug window).
            struct StreamGroup {
                std::vector<std::ostream*> streams;
                bool bDebugStreamEnabled{false};
            
                ///Write stream. Forwards to each referenced stream, and optionally debug output.
                template<typename T>
                StreamGroup& operator<<(const T &t) {
                    if (bDebugStreamEnabled)
                    {
                        std::ostringstream oss;
                        oss << t;
                        OutputDebugStringA(oss.str().c_str());
                    }

                    for (auto &stream : streams)
                        stream << t;

                    return *this;
                }
            };

            StreamGroup streamGroup;    ///<Group of streams (and debug out) attached to this log.
            std::string prefix;         ///<Prefix for each log message. Usually the log level.
            bool bFirstWrite{true};     ///<Has log been written yet? If not, don't prepend EOL.

        public:
            void attachStream(std::ostream &_stream) {
                streamGroup.streams.push_back(&_stream);
            }

            void enableDebugStream(bool b = true) { streamGroup.bDebugStreamEnabled = b; }

            ///Write stream.
            template<typename T>
            StreamGroup& operator<<(const T &t) {
                if (bFirstWrite)
                    bFirstWrite = false;
                else
                    streamGroup << "\n";

                ///@todo write file and line (but how, it would always report this line! Do we need to use macros?)
                streamGroup << prefix;
                streamGroup << t;
                return streamGroup;
            }

            static Logger i3log_instance;           ///<Log a normal info message, always defined.
            static Logger i3logWarning_instance;    ///<Log a warning, always defined.
            static Logger i3logError_instance;      ///<Log an error, always defined.
            ///Log a debug message, always defined.
            ///@attention This logger always exists (but might not be directed to any stream).
            ///           To disable in debug builds, wrap inside I3DEBUG_ONLY() macro.
            static Logger i3logDebug_instance;
        }; //class Logger
    } //namespace core
} //namespace i3

#endif //I3LOGGER_H
