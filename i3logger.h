#ifndef I3LOGGER_H
#define I3LOGGER_H

#include <memory>
#include <vector>
#include <ostream>

namespace i3 {
    using std::shared_ptr;
    using std::vector;
    using std::ostream;

    ///Logger stream class for streaming to multiple streams.
    class Logger {
        vector<shared_ptr<ostream>> streams;   ///<Streams which are attached.
    public:
        ///Attach a stream. The stream is not owned by this object, but must not destruct during this object's lifetime.
        void attachStream(shared_ptr<ostream> pStream) {
            streams.push_back(pStream);
        }

        ///Stream a value to all attached streams.
        template<typename T>
        Logger& operator<<(T& x) {
            foreach(auto pS : streams) {
                *pS << x;
            }
        }
    };
}

#endif //I3LOGGER_H
