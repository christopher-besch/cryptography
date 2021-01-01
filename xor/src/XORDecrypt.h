#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <Utils.h>

#include "XORDecrypted.h"

int character_to_int(char character, int check_base = 36, bool error = true);

void remove_unsupported_characters(std::string &str);

std::vector<std::string> get_encrypted_numbers(std::string &str, char delimiter, int digit_length);

long long decrypt_number(std::string digit_str, int base, int key = 0);

void try_decrypt(std::vector<XORDecrypted> &results, std::vector<std::string> &encrypted_numbers, char test_delimiter, int test_char_length, int highest_character);

std::vector<XORDecrypted> try_xor_decryption(std::string cipher);

struct XORDecrypted
{
    std::string m_decrypted_text;
    char m_delimiter;
    int m_char_length;
    int m_key;
    int m_base;
};

class XORDecrypt
{
private:
    std::string m_cipher;
    std::vector<XORDecrypted> decryptions;

    // decryptions with these parameters are preferred
    std::vector<char> m_requested_delimiters;
    std::vector<int> m_requested_char_lengths;
    std::vector<int> m_requested_keys;
    std::vector<int> m_requested_bases;

public:
    XORDecrypt(std::string &cipher)
        : m_cipher(cipher) {}

    void set_requested_delimiters(std::vector<char> &delimiters) { m_requested_delimiters = delimiters; }
    void set_requested_char_lengths(std::vector<int> &char_length) { m_requested_char_lengths = char_length; }
    void set_requested_keys(std::vector<int> &keys) { m_requested_keys = keys; }
    void set_requested_bases(std::vector<int> &bases) { m_requested_bases = bases; }

    void create_decryptions();
};
