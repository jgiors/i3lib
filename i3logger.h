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

        ///Stream class for streaming to multiple stream-like objects.
        template<typename... T>
        class MultiStream {
            std::tuple<T...> streams;   ///<Attached stream-like objects.

            template<int N>
            void print(std::string s) {
                print<N-1>(s);
                std::get<N-1>(streams) << s;
            }

            template<>
            void print<0>(std::string s) {}

        public:
            MultiStream() {}
            //MultiStream(std::tuple<T...> _streams) : streams(_streams) {}
            MultiStream(T... _streams) : streams(_streams) {}

            ///Stream a value to all attached streams.
            template<typename Value>
            MultiStream& operator<<(Value v) {
                std::string s(v);
                print<std::tuple_size<decltype(streams)>::value>(s);
                return *this;
            }
        };

        ///Primary logger class. Use the global instances (below) for logging.
        class Logger {
            //static MultiStream dummyMultiStream;
            std::string prefix;
            bool bWritten{ false };
        public:
            std::shared_ptr<MultiStream> pMultiStream{ NULL };

            Logger() = delete;
            Logger(std::string _prefix) : prefix(_prefix) {}

            template<typename T>
            MultiStream& operator<<(T &x) {
                ///@todo Add a dummy stream to use in this case, a dummy multistream does not work because it should be tmplated
                //if (!pMultiStream)
                //    return dummyMultiStream;

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
