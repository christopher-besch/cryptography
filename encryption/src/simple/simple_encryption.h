#pragma once

#include <algorithm>
#include <iostream>
#include <string>

#include "utils.h"

#include "encrypted.h"

class SimpleEncrypted : public Encrypted
{
public:
    SimpleEncrypted(std::string encrypted_string)
        : Encrypted(encrypted_string) {}
};

// todo: could also be implemented using transformations
inline SimpleEncrypted fence_encrypt(std::string str)
{
    std::string first, second;
    first.reserve(str.size() / 2);
    second.reserve(str.size() / 2);

    // add first character to first, second to second, third to first and so on
    for (int idx = 0; idx < str.size(); ++idx)
    {
        if (idx % 2)
            second.push_back(str[idx]);
        else
            first.push_back(str[idx]);
    }
    // return second glued to first
    return { first.append(second) };
}
