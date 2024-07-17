///@file
#include <Windows.h>
#include <debugapi.h>
#include <ostream>
#include <fstream>
#include "i3logger.h"

#include <iostream>

using i3::core::logger::Log;

namespace {

} //anonymous namespace

int main()
{   
    Log.attachDebugPrint();
    Log.attachStream(std::cerr);
    Log << "Hello World!\n";
    return 0;
}
