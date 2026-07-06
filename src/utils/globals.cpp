#include "utils/utils.hpp"

#ifdef DEBUG_BUILD
	LogLevel	current_level = DEBUG;
#else
	LogLevel	current_level = WARNING;
#endif
