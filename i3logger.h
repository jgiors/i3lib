#ifndef I3LOGGER_H
#define I3LOGGER_H
///@file

#include <memory>
#include <vector>
#include <ostream>

namespace i3 {
    namespace core {
        using std::shared_ptr;
        using std::vector;
        using std::ostream;

        ///Logger stream class for streaming to multiple streams.
        class Logger {
            vector<ostream*> streams;   ///<Streams which are attached.

        public:
            ///Attach a stream.
            ///@attention Currently, streams can not be detached.
            void attach(ostream &stream) {
                streams.push_back(&stream);
            }

            ///Stream a value to all attached streams.
            template<typename T>
            Logger& operator<<(T& x) {
                for (auto p : streams) {
                    *p << x;
                }
                return *this;
            }
        };
    } //namespace core
} //namespace i3

#endif //I3LOGGER_H
