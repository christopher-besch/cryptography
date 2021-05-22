#pragma once

#include <string>

// abstract base class for encrypted cipher
class Encrypted
{
protected:
    std::string m_encrypted_str;

public:
    Encrypted(std::string encrypted_str)
        : m_encrypted_str(encrypted_str) {}

    virtual ~Encrypted() = default;

    const std::string& get_encrypted_str() const
    {
        return m_encrypted_str;
    }
};
