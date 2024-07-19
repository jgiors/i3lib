///@file
#include <Windows.h>
#include <debugapi.h>
#include <ostream>
#include <fstream>
#include <filesystem>
#include "i3lib.h"

#include <iostream>

using std::cerr;
using i3::core::Logger;

namespace {
    constexpr auto logFilename = "i3log.log";
} //anonymous namespace

int main()
{
    Logger::i3logErr_instance.enableDebugStream();
    Logger::i3logErr_instance.attachStream(cerr);

    OutputDebugStringA("****************************** i3log debug output stream ******************************");
    cerr << "****************************** i3log console stream ******************************";

    bool bNewFile = !std::filesystem::exists(logFilename);
    std::ofstream logFile(logFilename, std::ios_base::ate);
    if (!bNewFile)
        logFile << "\n\n";
    logFile << "******************** i3log.log: Beginning new execution log. ********************";

    Logger::i3logErr_instance.attachStream(logFile);

    i3logErr << "NOT ACTUALLY AN ERRROR: Error logging is set up.";

    Logger::i3logWarn_instance.enableDebugStream();
    Logger::i3logWarn_instance.attachStream(cerr);
    Logger::i3logWarn_instance.attachStream(logFile);

    i3logWarn << "NOT ACTUALLY A WARNING: Warning logging is set up.";

    Logger::i3log_instance.enableDebugStream();
    Logger::i3log_instance.attachStream(cerr);
    Logger::i3log_instance.attachStream(logFile);

    i3log << "Normal level logging is set up.";
    
    Logger::i3logDebug_instance.enableDebugStream();
    Logger::i3logDebug_instance.attachStream(cerr);
    Logger::i3logDebug_instance.attachStream(logFile);

    i3logDebug << "Debug logging is set up.";

    return 0;
}
