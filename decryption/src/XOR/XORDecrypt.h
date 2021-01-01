#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <Utils.h>

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

    std::string get_header()
    {
        return "delimiter/char length\tkey\tbase";
    }

    operator std::string()
    {
        std::stringstream stats;
        if (delimiter)
            stats << delimiter << "\t";
        else
            stats << char_length << "\t";
        stats << key << "\t" << base << "\t" << decrypted_str;
        return stats.str();
    }
};

// for one cipher and all valid decryption options
class XORDecrypt
{
private:
    std::string m_cipher;
    // cipher with all possible delimiters removed
    std::string m_cipher_chars_only;
    std::vector<XORDecrypted> m_decryptions;

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
    // todo: there might be an off-by-one error here
    int character_to_int(char character, int check_base = 36, bool error = true);
    void preprocess();
    std::vector<std::string> cut_cipher_with_char_length(int char_length);
    std::vector<std::string> cut_cipher_with_delimiter(char delimiter);
    long long decrypt_number(std::string digit_str, int base, int key);
    void try_decrypt(std::vector<std::string> &encrypted_numbers, char test_delimiter, int test_char_length);
    void create_decryptions();
    void calculate_score();

public:
    XORDecrypt(std::string &cipher)
        : m_cipher(cipher), m_cipher_chars_only(cipher)
    {
        preprocess();
        create_decryptions();
    }

    void set_requested_delimiters(std::vector<char> &delimiters) { m_requested_delimiters = delimiters; }
    void set_requested_char_lengths(std::vector<int> &char_length) { m_requested_char_lengths = char_length; }
    void set_requested_keys(std::vector<int> &keys) { m_requested_keys = keys; }
    void set_requested_bases(std::vector<int> &bases) { m_requested_bases = bases; }

    void add_requested_delimiter(char delimiters) { m_requested_delimiters.push_back(delimiters); }
    void add_requested_char_length(int char_length) { m_requested_char_lengths.push_back(char_length); }
    void add_requested_key(int keys) { m_requested_keys.push_back(keys); }
    void add_requested_base(int bases) { m_requested_bases.push_back(bases); }

    std::vector<XORDecrypted> get_best_decryptions()
    {
    }
};