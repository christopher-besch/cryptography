#pragma once

#include <string>

#include <Decrypted.h>

class XORDecrypted : public Decrypted
{
private:
    int m_key;
    int m_base;

private:
    virtual void calculate_score() override;

public:
    XORDecrypted(std::string decrypted_text, char delimiter, int char_length, int key, int base)
        : Decrypted(decrypted_text, delimiter, char_length), m_key(key), m_base(base) {}

    virtual std::string get_type() const override { return "XOR Cipher"; }
    int get_key() const { return m_key; }
    int get_base() const { return m_base; }
};
