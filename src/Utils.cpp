#include "Utils.hpp"
#include <cstdlib>
#include <string>

int Utils::stringToInt(const std::string& str)
{ 
    // Convert to long int, specify base 10 (decimal)
    return static_cast<int>(std::strtol(str.c_str(), NULL, 10));
}

