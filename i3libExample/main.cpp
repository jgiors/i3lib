///@file
#include <Windows.h>
#include <debugapi.h>
#include <ostream>
#include <fstream>
#include "i3logger.h"

#include <iostream>

using i3::core::logger::Log;

int main()
{   
    Log.attachStream(std::cerr);
    Log.attachDebugPrint();    
    Log << "Hello World!\n";
    return 0;
}
