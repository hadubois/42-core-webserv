#pragma once

#include "ansi_colors.hpp"
#include <iostream>

#ifdef LOG
# define DEBUG(x) std::cerr << YELLOW "["__TIME__"]" "[DEBUG] " RST << x << std::endl
# define ERROR(x) std::cerr << RED "[ERROR] " RST << x << std::endl
# define INFO(x)  std::cerr << x << std::endl << RST
#else
# define DEBUG(x)
# define ERROR(x)
# define INFO(x)
#endif
