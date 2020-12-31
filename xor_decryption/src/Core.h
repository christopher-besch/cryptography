#pragma once

#include <string>
#include <algorithm>
#include <iostream>

#ifdef DEBUG
#define raise_error(msg)                                                                                                  \
    {                                                                                                                     \
        std::cerr << msg << " (in: " << __FILE__ << ":" << __LINE__ << "; in function: " << __func__ << ")" << std::endl; \
        std::exit(EXIT_FAILURE);                                                                                          \
    }
#else
#define raise_error(msg)               \
    {                                  \
        std::cerr << msg << std::endl; \
        std::exit(EXIT_FAILURE);       \
    }
#endif

int checked_stoi(std::string str)
{
    try
    {
        return std::stoi(str);
    }
    catch (std::invalid_argument ex)
    {
        raise_error("Can't convert \"" << str << "\" to int!");
    }
}
