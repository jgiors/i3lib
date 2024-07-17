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
        namespace logger {
            ///Abstract base for printing a string.
            struct Printer {
                virtual void print(std::string s) = 0;
            };

            ///Print to the debug stream. On Windows, prints to debug output/window.
            struct DebugPrinter : public Printer {
                virtual void print(std::string s) {
                    OutputDebugStringA(s.c_str());
                }
            };

            ///Print to a debug stream.
            class StreamPrinter : public Printer {
                std::ostream &stream;
            public:
                StreamPrinter(std::ostream &_stream) : stream(_stream) {}
                virtual void print(std::string s) {
                    stream << s;
                }
            };

            ///Primary logger class. Use the global instances (below) for logging.
            class Logger {
                struct PrintGroup {
                    std::vector<std::unique_ptr<Printer>> pPrinters;
                
                    template<typename T>
                    PrintGroup& operator<<(T t) {
                        std::string s(t);
                        for (auto &p : pPrinters) {
                            p->print(s);
                        }
                        return *this;
                    }
                };

                PrintGroup printGroup;
                std::string prefix;
                bool bWritten{ false };
            public:
                Logger() = delete;
                Logger(std::string _prefix) : prefix(_prefix) {}

                void attachStream(std::ostream &_stream) {
                    printGroup.pPrinters.push_back(std::unique_ptr<Printer>(new StreamPrinter(_stream)));
                }

                void attachDebugPrint() { printGroup.pPrinters.push_back(std::unique_ptr<Printer>(new DebugPrinter())); }

                template<typename T>
                PrintGroup& operator<<(T &x) {
                    if (!bWritten) {
                        printGroup << "\n";
                        bWritten = true;
                    }
                    ///@todo write file and line
                    printGroup << prefix;
                    printGroup << x;
                    return printGroup;
                }
            };

            extern Logger Log;         ///<Log a normal info message.
            extern Logger LogWarning;  ///<Log a warning.
            extern Logger LogError;    ///<Log an error.
            ///Log a debug message.
            ///@attention This logger always exists (but might not be directed to any stream).
            ///           To disable in debug builds, wrap inside I3DEBUG_ONLY() macro.
            extern Logger LogDebug;
        } //namespace logger
    } //namespace core
} //namespace i3

#endif //I3LOGGER_H
