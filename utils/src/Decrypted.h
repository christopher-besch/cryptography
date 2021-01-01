#pragma once

#include <string>

// for all types of decryptions of the cipher
class Decrypted
{
protected:
    std::string m_decrypted_text;

    char m_delimiter;
    int m_char_length;
    // the higher the better the decryption
    float m_score;
    bool m_score_calculated;

protected:
    virtual void calculate_score() = 0;

public:
    Decrypted(std::string decrypted_text, char delimiter, int char_length)
        : m_decrypted_text(decrypted_text), m_delimiter(delimiter), m_char_length(char_length) {}

    const std::string get_text() const { return m_decrypted_text; }
    const char get_delimiter() const { return m_delimiter; }
    const int get_char_length() const { return m_char_length; }
    const int get_score();

    virtual std::string get_type() const = 0;

    virtual bool operator<(Decrypted &other) { return get_score() < other.get_score(); }
    virtual bool operator<=(Decrypted &other) { return get_score() <= other.get_score(); }
    virtual bool operator>(Decrypted &other) { return get_score() > other.get_score(); }
    virtual bool operator>=(Decrypted &other) { return get_score() >= other.get_score(); }
    virtual bool operator==(Decrypted &other) { return get_score() == other.get_score(); }
    virtual bool operator!=(Decrypted &other) { return get_score() != other.get_score(); }
};
