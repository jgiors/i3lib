///@file
#include <Windows.h>
#include <debugapi.h>
#include <ostream>
#include <fstream>
#include "i3logger.h"

#include <iostream>

using i3::core::MultiStream;
using i3::core::Log;

int main()
{
    Log.pMultiStream = std::shared_ptr<MultiStream>(new MultiStream{ &std::cerr });
    Log << "Hello World!\n";
    return 0;
}
