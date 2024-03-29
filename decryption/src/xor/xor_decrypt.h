#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "utils.h"

#include "decrypted.h"
#include "goodness/library_search.h"

// for one valid decryption option
struct XORDecrypted : public Decrypted
{
    char delimiter;
    int  char_length;
    int  key;
    int  base;

    XORDecrypted() {}
    XORDecrypted(std::string decrypted_str, int score, char delimiter, int char_length, int key, int base)
        : Decrypted(decrypted_str, score), delimiter(delimiter), char_length(char_length), key(key), base(base) {}

    // todo: better alignment
    virtual void create_report() override
    {
        std::stringstream report_ss;
        if (delimiter == '\0')
            report_ss << decrypted_str << "\t-XOR, Base " << base << ", Key " << key << ", CharLength " << char_length << ", Score " << score;
        else
            report_ss << decrypted_str << "\t-XOR, Base " << base << ", Key " << key << ", Delimiter '" << delimiter << "', Score " << score;
        report = report_ss.str();
    }
};

// for one xor cipher and all valid decryption options
class XORDecryptor : public Decryptor
{
private:
    std::vector<XORDecrypted> m_decryptions;

    // cipher with all possible delimiters removed
    std::string m_cipher_chars_only;

    // when there are elements in these, only search for decryptions with these settings
    std::vector<char> m_requested_delimiters;
    std::vector<int>  m_requested_char_lengths;
    std::vector<int>  m_requested_keys;
    std::vector<int>  m_requested_bases;

    // all characters in the cipher that can't be decrypted
    std::vector<char> m_possible_delimiters;
    // the highest decryptable character defines the lower limit of the possible bases
    int m_smallest_base;

private:
    // convert encrypted char-digit in base <check_base> to int
    int character_to_int(char character, int check_base = 36, bool error = true);
    // find possible delimiters and smallest possible base
    void preprocess();
    // cut cipher into individual encrypted numbers by their length
    std::vector<std::string> cut_cipher_with_char_length(int char_length);
    // cut cipher into individual encrypted numbers by delimiter
    std::vector<std::string> cut_cipher_with_delimiter(char delimiter);
    // convert encrypted number in base <base> to int
    long long decrypt_number(std::string digit_str, int base, int key);
    // decrypt encrypted numbers with settings found in the template decrypt, add decryption to new copy of that decrypt and store it
    void decrypt(std::vector<std::string>& encrypted_numbers, XORDecrypted& template_decrypt);
    // decrypt encrypted numbers, trying every base and key possible
    void test_decryptions(std::vector<std::string>& encrypted_numbers, XORDecrypted& template_decrypt);

    // when a certain setting is requested, everything else won't be tested
    bool is_to_test_delimiter(char delim)
    {
        // when a char_length is defined but no delimiter, any delimiter has to be defined as well to be tested
        return (m_requested_delimiters.empty() && m_requested_char_lengths.empty()) ||
               std::find(m_requested_delimiters.begin(), m_requested_delimiters.end(), delim) != m_requested_delimiters.end();
    }
    bool is_to_test_char_length(int char_length)
    {
        // when a delimiter is defined but no char_length, any char_length has to be defined as well to be tested
        return (m_requested_char_lengths.empty() && m_requested_delimiters.empty()) ||
               std::find(m_requested_char_lengths.begin(), m_requested_char_lengths.end(), char_length) != m_requested_char_lengths.end();
    }
    bool is_to_test_key(int key)
    {
        return m_requested_keys.empty() ||
               std::find(m_requested_keys.begin(), m_requested_keys.end(), key) != m_requested_keys.end();
    }
    bool is_to_test_base(int base)
    {
        return m_requested_bases.empty() ||
               std::find(m_requested_bases.begin(), m_requested_bases.end(), base) != m_requested_bases.end();
    }

public:
    XORDecryptor(const std::string& cipher, const LibrarySearch& dictionary)
        : Decryptor(cipher, dictionary), m_cipher_chars_only(cipher)
    {
        preprocess();
    }
    // copies are not allowed
    XORDecryptor(const XORDecryptor&) = delete;
    XORDecryptor& operator=(const XORDecryptor&) = delete;

    void add_requested_delimiter(char delimiter)
    {
        if (character_to_int(delimiter, 36, false) != -1)
            raise_error("The provided delimiter '" << delimiter << "' is invalid!");
        m_requested_delimiters.push_back(delimiter);
    }
    void add_requested_char_length(int char_length)
    {
        if (m_cipher_chars_only.size() % char_length)
            raise_error("The provided char length '" << char_length << "' is invalid with the cipher " << m_cipher_chars_only << "!");
        m_requested_char_lengths.push_back(char_length);
    }
    void add_requested_key(int key)
    {
        if (key < 0 || key > 255)
            raise_error("The provided key '" << key << "' is invalid!");
        m_requested_keys.push_back(key);
    }
    void add_requested_base(int base)
    {
        if (base < 2 || base > 36)
            raise_error("The provided base '" << base << "' is invalid!");
        m_requested_bases.push_back(base);
    }

    std::vector<XORDecrypted>& get_decryptions()
    {
        return m_decryptions;
    }

    virtual void create_decryptions(int amount = 0) override;
};
