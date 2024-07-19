#ifndef I3LOGGER_H
#define I3LOGGER_H
///@file

#include <Windows.h>
#include <memory>
#include <initializer_list>
#include <vector>
#include <tuple>
#include <string>
#include <cstring>
#include <sstream>
#include "i3defs.h"

///Filename and line number format.
///@note The __FILE__ part is not concatenated with line number as a size optimization.
///Letting __FILE__ stand alone means the linker can consoldiate all instance into one string.
///That would not be possible with line numbers appended to each instance.
#define I3LOG_FILE_AND_LINE     i3::core::Logger::extractFilename(__FILE__) << "(" << I3STRINGIZE(__LINE__) << "): "

#define I3LOG_PREFIX            I3LOG_FILE_AND_LINE
#define I3LOG_WARNING_PREFIX    I3LOG_FILE_AND_LINE "WARNING! "
#define I3LOG_ERROR_PREFIX      I3LOG_FILE_AND_LINE "!!! ERROR !!! "
#define I3LOG_DEBUG_PREFIX      I3LOG_FILE_AND_LINE "[DEBUG] "

///Default informational log stream.
#define i3log           i3::core::Logger::i3log_instance << I3LOG_PREFIX
///Warning log stream.
#define i3logWarn       i3::core::Logger::i3logWarn_instance << I3LOG_WARNING_PREFIX
///Error log stream.
#define i3logErr        i3::core::Logger::i3logErr_instance <<  I3LOG_ERROR_PREFIX

///Debug log stream. Compile out in debug (assuming compiler properly optimizes it out).
#if DEBUG
#   define i3logDebug   i3::core::Logger::i3logDebug_instance << I3LOG_DEBUG_PREFIX
#else
#   define i3logDebug   if (false) cerr
#endif

namespace i3 {
    namespace core {
        ///Primary logger class. Use macros writing to the global instances.
        class Logger {
            ///Group of streams. Streaming to this object forwards to each referenced stream.
            ///Also supports debug output stream (on Windows, this is routed to the debug window).
            struct StreamGroup {
                std::vector<std::ostream*> streams; ///<Attached output streams.
                bool bDebugStreamEnabled{false};    ///<Write to debug output?
            
                ///Stream Write. Forwards to each referenced stream, and optionally debug output.
                template<typename T>
                StreamGroup& operator<<(const T &t) {
                    if (bDebugStreamEnabled)
                    {
                        std::ostringstream oss;
                        oss << t;
                        OutputDebugStringA(oss.str().c_str());
                    }

                    for (auto &stream : streams)
                        *stream << t;

                    return *this;
                }
            };

            StreamGroup streamGroup;    ///<Group of streams (and debug out) attached to this log.
            bool bFirstWrite{true};     ///<Has log been written yet? If not, don't prepend EOL.

        public:
            void attachStream(std::ostream &_stream) {  ///<Attach an output stream.
                streamGroup.streams.push_back(&_stream);
            }

            void enableDebugStream(bool b = true) { streamGroup.bDebugStreamEnabled = b; }

            ///Stream write. Except for the first write, prepends a newline bacause there is no
            ///way to postpend a newline.
            template<typename T>
            StreamGroup& operator<<(const T &t) { return streamGroup << "\n" << t; }

            ///Extract filename from full path. Used to streamline logging.
            static constexpr std::string extractFilename(const std::string filepath) {
                size_t pos = filepath.find_last_of('\\');
                return (pos == std::string::npos) ? filepath : filepath.substr(pos + 1);
            }

            static Logger i3log_instance;           ///<Log a normal info message, always defined.
            static Logger i3logWarn_instance;    ///<Log a warning, always defined.
            static Logger i3logErr_instance;      ///<Log an error, always defined.
            ///Log a debug message, always defined.
            ///@attention This logger always exists (but might not be directed to any stream).
            ///           To disable in debug builds, wrap inside I3DEBUG_ONLY() macro.
            static Logger i3logDebug_instance;
        }; //class Logger
    } //namespace core
} //namespace i3

#endif //I3LOGGER_H
