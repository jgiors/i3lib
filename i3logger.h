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

#define i3log           i3::core::logger::log_always        ///<Default log stream.
#define i3logWarning    i3::core::logger::logWarning_always ///<Warning log stream.
#define i3logError      i3::core::logger::logError_always   ///<Error log stream.

///Debug log stream. Compile out in debug (assuming compiler properly optimizes it out).
#if DEBUG
#   define i3logDebug   i3::core::logger::logDebug_always
#else
#   define i3logDebug   if (false) cerr
#endif

///@todo complete documentation in this file

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

            ///Primary logger class. Use global instances for logging.
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

            ///@todo add macros to turn these on/off

            extern Logger log_always;           ///<Log a normal info message, always defined.
            extern Logger logWarning_always;    ///<Log a warning, always defined.
            extern Logger logError_always;      ///<Log an error, always defined.
            ///Log a debug message, always defined.
            ///@attention This logger always exists (but might not be directed to any stream).
            ///           To disable in debug builds, wrap inside I3DEBUG_ONLY() macro.
            extern Logger logDebug_always;
        } //namespace logger
    } //namespace core
} //namespace i3

#endif //I3LOGGER_H
