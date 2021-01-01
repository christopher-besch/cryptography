#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>

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

inline int checked_stoi(std::string str)
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

inline void remove_chars(std::string &str, std::vector<char> chars)
{
    for (int idx = str.size() - 1; idx >= 0; idx--)
        if (std::find(str.begin(), str.end(), str[idx]) != str.end())
            str.erase(idx, 1);
}
