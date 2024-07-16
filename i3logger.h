#ifndef I3LOGGER_H
#define I3LOGGER_H
///@file

#include <Windows.h>
#include <memory>
#include <initializer_list>
#include <vector>
#include <string>
#include <sstream>

namespace i3 {
    namespace core {

        struct DebugStream {
            ///Stream to debug window or other appropriate stream.
            template<typename T>
            DebugStream& operator<<(T& x) {
                std::ostringstream s;
                s << x;
                OutputDebugStringA(s.str().c_str());
                return *this;
            }
        };

        ///Stream class for streaming to multiple streams.
        class MultiStream {
            std::vector<std::ostream*> streams;   ///<Streams which are attached.

        public:
            MultiStream() = delete;
            MultiStream(std::initializer_list<std::ostream*> _streams) : streams(_streams) {}

            ///Stream a value to all attached streams.
            template<typename T>
            MultiStream& operator<<(T &x) {
                for (auto p : streams) {
                    *p << x;
                }
                return *this;
            }
        };

        ///Primary logger class. Use the global instances (below) for logging.
        class Logger {
            std::string prefix;
            bool bWritten{ false };
        public:
            std::shared_ptr<MultiStream> pMultiStream{ NULL };

            Logger() = delete;
            Logger(std::string _prefix) : prefix(_prefix) {}

            template<typename T>
            MultiStream& operator<<(T &x) {
                if (!pMultiStream)
                    return *this;

                if (!bWritten) {
                    *pMultiStream << "\n";
                    bWritten = true;
                }
                ///@todo write file and line
                *pMultiStream << prefix;
                *pMultiStream << x;
                return *pMultiStream;
            }
        };

        extern Logger Log;         ///<Log a normal info message.
        extern Logger LogWarning;  ///<Log a warning.
        extern Logger LogError;    ///<Log an error.
        ///Log a debug message.
        ///@attention This logger always exists (but might not be directed to any stream).
        ///           To disable in debug builds, wrap inside I3DEBUG_ONLY() macro.
        extern Logger LogDebug;
    } //namespace core
} //namespace i3

#endif //I3LOGGER_H
