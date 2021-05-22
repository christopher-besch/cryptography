#pragma once

#include <string>

#include "goodness/library_search.h"

// abstraction for one valid decryption option
struct Decrypted
{
    std::string decrypted_str;
    // used to compare different decryptions
    float score;
    // representing this decryption in console or file output
    std::string report;

    Decrypted() {}
    Decrypted(std::string decrypted_str, int score)
        : decrypted_str(decrypted_str), score(score) {}

    bool operator<(const Decrypted& other) const
    {
        return score < other.score;
    }
    bool operator<=(const Decrypted& other) const
    {
        return score <= other.score;
    }
    bool operator>(const Decrypted& other) const
    {
        return score > other.score;
    }
    bool operator>=(const Decrypted& other) const
    {
        return score >= other.score;
    }
    bool operator==(const Decrypted& other) const
    {
        return score == other.score;
    }
    bool operator!=(const Decrypted& other) const
    {
        return score != other.score;
    }

    virtual void create_report() = 0;
};

// abstraction for single type of cipher and all valid decryption options
class Decryptor
{
protected:
    std::string const    m_cipher;
    LibrarySearch const& m_dictionary;
    // how many decryptions should be stored, 0 for as many as possible
    int m_amount;

public:
    Decryptor(const std::string& cipher, const LibrarySearch& dictionary)
        : m_cipher(cipher), m_dictionary(dictionary) {}
    // copies are not allowed
    Decryptor(const Decryptor&) = delete;
    Decryptor& operator=(const Decryptor&) = delete;

    virtual ~Decryptor() = default;

    // find <amount> ways to decrypt <m_cipher>
    virtual void create_decryptions(int amount = 0) = 0;
};
