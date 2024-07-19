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

    void initLogger(std::ofstream &logFile)
    {
        Logger::i3logErr_instance.enableDebugStream();
        Logger::i3logErr_instance.streamGroup << I3LOG_FILE_AND_LINE "Debug output log stream established.\n";

        Logger::i3logErr_instance.attachStream(cerr);
        Logger::i3logErr_instance.streamGroup << I3LOG_FILE_AND_LINE "Standard console cerr stream established.";

        logFile.open(logFilename, std::ios_base::out | std::ios_base::app);
        bool bLogFileIsEmpty = (std::filesystem::file_size(logFilename) == 0);
        if (bLogFileIsEmpty)
        {
            logFile << I3LOG_FILE_AND_LINE << "*************\n";
            logFile << I3LOG_FILE_AND_LINE << "* i3log.log *\n";
            logFile << I3LOG_FILE_AND_LINE << "*************\n";
        }
        else
        {
            logFile << "\n" << I3LOG_FILE_AND_LINE << "\n";
            logFile << I3LOG_FILE_AND_LINE << "******************************************\n";
            logFile << I3LOG_FILE_AND_LINE << "* Appending output to existing i3log.log *\n";
            logFile << I3LOG_FILE_AND_LINE << "******************************************\n";
        }

        logFile << I3LOG_FILE_AND_LINE;

        Logger::i3logErr_instance.attachStream(logFile);

        Logger::i3logErr_instance << I3LOG_FILE_AND_LINE "Error logging established.";

        Logger::i3logWarn_instance.enableDebugStream();
        Logger::i3logWarn_instance.attachStream(cerr);
        Logger::i3logWarn_instance.attachStream(logFile);

        Logger::i3logWarn_instance << I3LOG_FILE_AND_LINE "Warning logging established.";

        Logger::i3log_instance.enableDebugStream();
        Logger::i3log_instance.attachStream(cerr);
        Logger::i3log_instance.attachStream(logFile);

        i3log << "Default logging established.";
        
        Logger::i3logDebug_instance.enableDebugStream();
        Logger::i3logDebug_instance.attachStream(cerr);
        Logger::i3logDebug_instance.attachStream(logFile);

        i3logDebug << "Debug logging established.";
    }
} //anonymous namespace

int main()
{
    std::ofstream logFile;
    initLogger(logFile);    
    return 0;
}
