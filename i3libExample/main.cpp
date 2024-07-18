///@file
#include <Windows.h>
#include <debugapi.h>
#include <ostream>
#include <fstream>
#include "i3lib.h"

#include <iostream>

namespace {

} //anonymous namespace

int main()
{   
    i3::core::Logger::i3log_instance.enableDebugStream();
    i3::core::Logger::i3log_instance.attachStream(std::cerr);
    i3log << "Hello world!\n";
    return 0;
}
