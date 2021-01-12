#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include <Utils.h>

#include "Encrypted.h"

class TransposeEncrypted : public Encrypted
{
private:
    int m_key;

public:
    TransposeEncrypted(std::string encrypted_string, int key)
        : Encrypted(encrypted_string), m_key(key) {}

    int get_key() const { return m_key; }
};

// equivalent to: fill row by row, read column by column like a snake
TransposeEncrypted plow_encrypt(const std::string &str, int original_column_count, char filler = '_');

// equivalent to: fill row by row, read column by column
TransposeEncrypted transpose_encrypt(const std::string &str, int original_column_count, char filler = '_');
