#pragma once

#include <string>

class Encrypted
{
protected:
    std::string m_encrypted_str;

public:
    Encrypted(std::string encrypted_str)
        : m_encrypted_str(encrypted_str) {}

    const std::string &get_encrypted_str() const { return m_encrypted_str; }
};
