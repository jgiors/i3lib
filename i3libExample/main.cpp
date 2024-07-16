///@file
#include <Windows.h>
#include <debugapi.h>
#include <ostream>
#include <fstream>
#include "i3logger.h"

#include <iostream>

///Log stream.
///@note "Log" is capitalized to prevent collision with mathematical log() function.
i3::core::MultiLogger Log;

int main()
{
    Log.attach(std::cerr);
    Log << "Hello World!\n";
}
