#pragma once

#include <string>

class Decrypted
{
private:
    std::string m_decrypted_text;

    char m_delimiter;
    int m_char_length;
    float m_score;

private:
    void calculate_score();

public:
    Decrypted(std::string decrypted_text, char delimiter, int char_length);

    std::string get_text() { return m_decrypted_text; }
    char get_delimiter() const { return m_delimiter; }
    int get_char_length() const { return m_char_length; }
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
    int m_key;
    int m_base;

public:
    XORDecrypted(std::string decrypted_text, char delimiter, int char_length, int key, int base)
        : Decrypted(decrypted_text, delimiter, char_length), m_key(key), m_base(base) {}

    virtual std::string get_type() const override { return "XOR Cipher"; }
    int get_key() const { return m_key; }
    int get_base() const { return m_base; }
};
