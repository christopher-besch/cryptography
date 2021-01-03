#pragma once

#include <iostream>
#include <string>
#include <algorithm>

#include <Utils.h>

struct XOREncrypted
{
    std::string encrypted_str;
    int base;
    int key;

    // todo: make better
    static std::string get_header()
    {
        return "key\tbase\tresult";
    }

    friend std::ostream &operator<<(std::ostream &lhs, const XOREncrypted &rhs)
    {
        lhs << rhs.key << "\t" << rhs.base << "\t" << rhs.encrypted_str;
        return lhs;
    }
};

std::string encrypt_number(int number, int base, int key = 0, int min_length = 1);

// get length of biggest ASCII char (255) in base <base>
inline int get_encrypted_char_length(int base)
{
    return encrypt_number(255, base).size();
}

XOREncrypted encrypt_text(std::string str, int base, int key = 0, char delimiter = '\0', bool add_0 = false);
