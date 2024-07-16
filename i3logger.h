#ifndef I3LOGGER_H
#define I3LOGGER_H
///@file

#include <memory>
#include <vector>
#include <string>
#include <sstream>

namespace i3 {
    namespace core {

        struct DebugStream {
            ///Stream to debug window or other appropriate stream.
            template<typename T>
            MultiStream& operator<<(T& x) {
                std::ostringstream
                return *this;
            }
        };

        ///Stream class for streaming to multiple streams.
        class MultiStream {
            std::vector<std::ostream*> streams;   ///<Streams which are attached.

        public:
            ///Attach a stream. The stream is not owned by the MultiLogger and must remain valid.
            ///@attention Currently, streams can not be detached.
            void attach(ostream &stream) {
                streams.push_back(&stream);
            }

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
            MultiStream multiStream;
            std::string prefix;
            bool bWritten;
        public:
            Logger() = delete;
            Logger(std::string _prefix) : prefix(_prefix) {}

            template<typename T>
            MultiStream& operator<<(T &x) {
                if (!bWritten) {
                    multiStream << "\n";
                    bWritten = true;
                }
                ///@todo write file and line
                multiStream << prefix;
                return multiStream << x;
            }
        };

        Logger Log;         ///<Log a normal info message.
        Logger LogWarning;  ///<Log a warning.
        Logger LogError;    ///<Log an error.
        ///Log a debug message.
        ///@attention This logger always exists (but might not be directed to any stream).
        ///           To disable in debug builds, wrap inside I3DEBUG_ONLY() macro.
        Logger LogDebug;
    } //namespace core
} //namespace i3

#endif //I3LOGGER_H
