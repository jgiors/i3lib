///@file
#include "i3logger.h"

i3::core::Logger i3::core::Log(" ");
i3::core::Logger i3::core::LogWarning(" WARNING! ");
i3::core::Logger i3::core::LogError(" !!! ERROR !!! ");
i3::core::Logger i3::core::LogDebug(" [DEBUG] ");

i3::core::MultiStream i3::core::Logger::dummyMultiStream;
