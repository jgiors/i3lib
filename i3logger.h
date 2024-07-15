#ifndef I3LOGGER_H
#define I3LOGGER_H

#include <vector>
#include <ostream>

using std::vector;
using std::ostream;

namespace i3 {
    class Logger {
        vector<ostream> streams;
    public:
        template<typename T>
        Logger& operator<<(T& x) {
            foreach(ostream s : streams) {
                s << x;
            }
        }
    };
}

#endif //I3LOGGER_H
