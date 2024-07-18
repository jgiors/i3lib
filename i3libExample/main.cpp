///@file
#include <Windows.h>
#include <debugapi.h>
#include <ostream>
#include <fstream>
#include "i3logger.h"

#include <iostream>

namespace {

} //anonymous namespace

int main()
{   
    i3log.attachDebugPrint();
    i3log.attachStream(std::cerr);
    i3log << "Hello World!\n";
    return 0;
}
