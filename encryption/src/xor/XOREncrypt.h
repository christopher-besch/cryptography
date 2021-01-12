#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include <Utils.h>

#include "Encrypted.h"

class XOREncrypted : public Encrypted
{
private:
    int m_base;
    int m_key;

public:
    XOREncrypted(std::string encrypted_string, int base, int key)
        : Encrypted(encrypted_string), m_base(base), m_key(key) {}

    int get_base() const { return m_base; }
    int get_key() const { return m_key; }
};

std::string xor_encrypt_number(int number, int base, int key = 0, int min_length = 1);

// get length of biggest ASCII char (255) in base <base>
inline int get_xor_encrypted_char_length(int base)
{
    return xor_encrypt_number(255, base).size();
}

XOREncrypted xor_encrypt(std::string str, int base, int key = 0, char delimiter = '\0', bool add_0 = false);
