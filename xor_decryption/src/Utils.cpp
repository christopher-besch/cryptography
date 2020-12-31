#include "Utils.h"

#include <string>
#include <iostream>

#include "Core.h"
#include "Utils.h"

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
