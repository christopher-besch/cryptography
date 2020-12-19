#pragma once

#include <string>

class Decrypted
{
private:
    std::string decrypted_text;

    char m_delimiter;
    int m_score;

private:
    void calculate_score();

public:
    Decrypted(std::string decrypted_text, char delimiter);

    std::string get_text() { return decrypted_text; }
    char get_delimiter() const { return m_delimiter; }
    int get_score() const { return m_score; }

    virtual std::string get_type() const = 0;

    virtual bool operator<(Decrypted &other) const { return m_score < other.m_score; }
    virtual bool operator<=(Decrypted &other) const { return m_score <= other.m_score; }
    virtual bool operator>(Decrypted &other) const { return m_score > other.m_score; }
    virtual bool operator>=(Decrypted &other) const { return m_score >= other.m_score; }
    virtual bool operator==(Decrypted &other) const { return m_score == other.m_score; }
    virtual bool operator!=(Decrypted &other) const { return m_score != other.m_score; }
};

class XORDecrypted : public Decrypted
{
private:
    int key;
    int base;

public:
    XORDecrypted(std::string decrypted_text, char delimiter, int key, int base)
        : Decrypted(decrypted_text, delimiter), key(key), base(base) {}

    virtual std::string get_type() const override { return "XOR Cipher"; }
    int get_key() const { return key; }
    int get_base() const { return base; }
};
