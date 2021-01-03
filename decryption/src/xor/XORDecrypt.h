#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "Utils.h"

#include "goodness/LibrarySearch.h"

// for one valid decryption option
struct XORDecrypted
{
    std::string decrypted_str;
    char delimiter;
    int char_length;
    int key;
    int base;
    // used to compare different decryption options
    int score;

    bool operator<(XORDecrypted &other) { return score < other.score; }
    bool operator<=(XORDecrypted &other) { return score <= other.score; }
    bool operator>(XORDecrypted &other) { return score > other.score; }
    bool operator>=(XORDecrypted &other) { return score >= other.score; }
    bool operator==(XORDecrypted &other) { return score == other.score; }
    bool operator!=(XORDecrypted &other) { return score != other.score; }

    // todo: make this better
    static std::string get_header()
    {
        return "score\tdelimiter/char_length\tkey\tbase\tresult";
    }

    friend std::ostream &operator<<(std::ostream &lhs, const XORDecrypted &rhs)
    {
        lhs << rhs.score << "\t";
        if (rhs.delimiter)
            lhs << "'" << rhs.delimiter << "'";
        else
            lhs << rhs.char_length;
        lhs << "\t\t\t";
        lhs << rhs.key << "\t" << rhs.base << "\t";

        // remove unprintable characters
        for (char character : rhs.decrypted_str)
            if (character >= ' ' || character <= '~')
                lhs << character;
        return lhs;
    }
};

// for one cipher and all valid decryption options
class XORDecrypt
{
private:
    std::string m_cipher;
    // cipher with all possible delimiters removed
    std::string m_cipher_chars_only;

    // how many decryptions should be stored, -1 for as many as possible
    int m_amount;
    std::vector<XORDecrypted> m_decryptions;

    LibrarySearch &m_dictionary;

    // decryptions with these parameters are preferred
    std::vector<char> m_requested_delimiters;
    std::vector<int> m_requested_char_lengths;
    std::vector<int> m_requested_keys;
    std::vector<int> m_requested_bases;

    // all characters in the cipher that can't be decrypted
    std::vector<char> m_possible_delimiters;
    // the highest decryptable character defines the lower limit of the base
    int m_smallest_base;

private:
    // convert digit in base <check_base> to int
    int character_to_int(char character, int check_base = 36, bool error = true);
    void preprocess();
    std::vector<std::string> cut_cipher_with_char_length(int char_length);
    std::vector<std::string> cut_cipher_with_delimiter(char delimiter);
    long long decrypt_number(std::string digit_str, int base, int key);
    void decrypt(std::vector<std::string> &encrypted_numbers, XORDecrypted &template_decrypt);
    void test_decryptions(std::vector<std::string> &encrypted_numbers, XORDecrypted &template_decrypt);

    // when a certain setting is requested, everything else won't be tested
    bool is_to_test_delimiter(char delim)
    {
        // when a char_length is defined but no delimiter, the delimiter has to be defined as well to be tested
        return (m_requested_delimiters.empty() && m_requested_char_lengths.empty()) || std::find(m_requested_delimiters.begin(), m_requested_delimiters.end(), delim) != m_requested_delimiters.end();
    }
    bool is_to_test_char_length(int char_length)
    {
        // when a delimiter is defined but no char_length, the char_length has to be defined as well to be tested
        return (m_requested_char_lengths.empty() && m_requested_delimiters.empty()) || std::find(m_requested_char_lengths.begin(), m_requested_char_lengths.end(), char_length) != m_requested_char_lengths.end();
    }
    bool is_to_test_key(int key)
    {
        return m_requested_keys.empty() || std::find(m_requested_keys.begin(), m_requested_keys.end(), key) != m_requested_keys.end();
    }
    bool is_to_test_base(int base)
    {
        return m_requested_bases.empty() || std::find(m_requested_bases.begin(), m_requested_bases.end(), base) != m_requested_bases.end();
    }

public:
    XORDecrypt(std::string &cipher, LibrarySearch &dictionary)
        : m_cipher(cipher), m_cipher_chars_only(cipher), m_dictionary(dictionary)
    {
        preprocess();
    }

    void set_requested_delimiters(std::vector<char> &delimiters) { m_requested_delimiters = delimiters; }
    void set_requested_char_lengths(std::vector<int> &char_length) { m_requested_char_lengths = char_length; }
    void set_requested_keys(std::vector<int> &keys) { m_requested_keys = keys; }
    void set_requested_bases(std::vector<int> &bases) { m_requested_bases = bases; }

    void add_requested_delimiter(char delimiter)
    {
        m_requested_delimiters.push_back(delimiter);
    }
    void add_requested_char_length(int char_length)
    {
        if (m_cipher_chars_only.size() % char_length)
            raise_error("The provided char length is invalid with the cipher " << m_cipher_chars_only << "!");
        m_requested_char_lengths.push_back(char_length);
    }
    void add_requested_key(int key)
    {
        if (key < 0 || key > 255)
            raise_error("The provided key " << key << " is invalid!");
        m_requested_keys.push_back(key);
    }
    void add_requested_base(int base)
    {
        if (base < 2 || base > 36)
            raise_error("The provided base " << base << " is invalid!");
        m_requested_bases.push_back(base);
    }

    void create_decryptions(int amount = -1);

    const std::vector<XORDecrypted> &get_decryptions() const { return m_decryptions; }
};
