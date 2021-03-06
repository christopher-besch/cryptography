#pragma once

#include <iostream>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <filesystem>

#ifdef LINUX
#define file_slash '/'
#endif
#ifdef WINDOWS
#define file_slash '\\'
#endif

// load files from executable location
// todo: doesn't work when loaded from PATH
inline std::string get_exec_dir(std::string execution_path)
{
    // cut at last slash
    for (int idx = execution_path.size() - 1; idx >= 0; idx--)
        if (execution_path[idx] == file_slash)
            return execution_path.substr(0, idx + 1);
    return "";
}

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
    catch (const std::invalid_argument ex)
    {
        raise_error("Can't convert \"" << str << "\" to int!");
    }
}

inline void remove_chars(std::string &str, std::vector<char> chars)
{
    for (int idx = str.size() - 1; idx >= 0; idx--)
        if (std::find(chars.begin(), chars.end(), str[idx]) != chars.end())
            str.erase(idx, 1);
}

inline void make_lower_case(std::string &str)
{
    for (char &character : str)
        if (character >= 'A' && character <= 'Z')
            character += 'a' - 'A';
}

template <typename T, typename R>
inline bool contains(T list, R element)
{
    return std::find(list.begin(), list.end(), element) != list.end();
}
