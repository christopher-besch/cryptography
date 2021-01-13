#pragma once

#include <iostream>
#include <string>
#include <algorithm>

#include <Utils.h>

#include "Encrypted.h"

class SimpleEncrypted : public Encrypted
{
public:
    SimpleEncrypted(std::string encrypted_string)
        : Encrypted(encrypted_string) {}
};

inline SimpleEncrypted fence_encrypt(std::string str)
{
    std::string first;
    std::string second;

    // add first character to first, second to second, third to first and so on
    for (int idx = 0; idx < str.size(); ++idx)
    {
        if (idx % 2)
            second.push_back(str[idx]);
        else
            first.push_back(str[idx]);
    }
    // return second glued to first
    return {first.append(second)};
}
