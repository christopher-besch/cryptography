#pragma once

#include <string>

#include "goodness/LibrarySearch.h"

// abstraction for one valid decryption option
struct Decrypted
{
    std::string decrypted_str;
    // used to compare different decryption options
    int score;

    Decrypted() {}
    Decrypted(std::string decrypted_str, int score)
        : decrypted_str(decrypted_str), score(score) {}

    bool operator<(const Decrypted &other) const { return score < other.score; }
    bool operator<=(const Decrypted &other) const { return score <= other.score; }
    bool operator>(const Decrypted &other) const { return score > other.score; }
    bool operator>=(const Decrypted &other) const { return score >= other.score; }
    bool operator==(const Decrypted &other) const { return score == other.score; }
    bool operator!=(const Decrypted &other) const { return score != other.score; }

    virtual std::string get_report() const = 0;
};

// abstraction for single type of cipher and all valid decryption options
class Decryptor
{
protected:
    std::string m_cipher;
    LibrarySearch &m_dictionary;
    // how many decryptions should be stored, 0 for as many as possible
    int m_amount;

public:
    Decryptor(std::string &cipher, LibrarySearch &dictionary)
        : m_cipher(cipher), m_dictionary(dictionary) {}
    // copies are not allowed
    Decryptor(const Decryptor &) = delete;
    Decryptor &operator=(const Decryptor &) = delete;

    // find <amount> ways to decrypt <m_cipher>
    virtual void create_decryptions(int amount = 0) = 0;
};
