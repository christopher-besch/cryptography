#include "XORDecrypt.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <Utils.h>

// convert char-digit in base <check_base> to int
int XORDecrypt::character_to_int(char character, int check_base, bool error)
{
    int digit = check_base;
    if (character >= '0' && character <= '9')
        digit = character - '0';
    else if (character >= 'a' && character <= 'z')
        digit = character - 'a' + 10;
    else if (character >= 'A' && character <= 'Z')
        digit = character - 'A' + 10;

    // check if character is valid in check_base
    if (digit >= check_base)
    {
        if (!error)
            return -1;
        else
            raise_error("'" << character << "' not supported with base " << std::to_string(static_cast<int>(check_base)));
    }
    return digit;
}

// find possible delimiters and smallest possible base
void XORDecrypt::preprocess()
{
    m_smallest_base = 0;
    for (char character : m_cipher)
    {
        // when a character causes character_to_int to fail, it might be a delimiter
        char decrypted_character = character_to_int(character, 36, false);
        // add each only once
        if (decrypted_character == -1 && std::find(m_possible_delimiters.begin(), m_possible_delimiters.end(), character) == m_possible_delimiters.end())
            m_possible_delimiters.push_back(character);

        // todo: off-by-one error?
        // e.g. when the character representing the highest number is '9', base 10 is the "smallest" possible base
        else if (decrypted_character + 1 > m_smallest_base)
            m_smallest_base = decrypted_character + 1;
    }
    // remove everything but the char-digits itself
    remove_chars(m_cipher_chars_only, m_possible_delimiters);
}

// cut cipher into individual encrypted numbers by length of each
std::vector<std::string> XORDecrypt::cut_cipher_with_char_length(int char_length)
{
    std::vector<std::string> encrypted_numbers;
    for (int idx = 0; idx < m_cipher_chars_only.size(); idx += char_length)
        encrypted_numbers.push_back(m_cipher_chars_only.substr(idx, char_length));
    return encrypted_numbers;
}

// cut cipher into individual encrypted numbers by delimiter
std::vector<std::string> XORDecrypt::cut_cipher_with_delimiter(char delimiter)
{
    std::vector<std::string> encrypted_numbers;
    std::stringstream ss_cipher(m_cipher);

    for (std::string buffer; std::getline(ss_cipher, buffer, delimiter);)
    {
        // remove everything but the characters itself
        remove_chars(buffer, m_possible_delimiters);
        if (!buffer.empty())
            encrypted_numbers.push_back(buffer);
    }
    return encrypted_numbers;
}

// convert encrypted number in base <base> to int
long long XORDecrypt::decrypt_number(std::string digit_str, int base, int key)
{
    long long result_char_code = 0;
    for (char character : digit_str)
        result_char_code = base * result_char_code + character_to_int(character, base, true);

    // xor decryption
    return result_char_code ^ key;
}

// decrypt encrypted numbers, trying every base and key possible
void XORDecrypt::try_decrypt(std::vector<std::string> &encrypted_numbers, char test_delimiter, int test_char_length)
{
    // test possible bases
    for (int test_base = m_smallest_base; test_base < 37; test_base++)
    {
        // todo: test more keys
        // test possible keys
        for (int test_key = 0; test_key < 256; test_key++)
        {
            bool possible = true;
            std::string decrypted_str = "";
            // decrypt each number
            for (std::string encrypted_number : encrypted_numbers)
            {
                // is decodable with extended ASCII?
                long long decrypted_number = decrypt_number(encrypted_number, test_base, test_key);
                if (decrypted_number > 255)
                {
                    possible = false;
                    break;
                }
                decrypted_str.push_back(static_cast<char>(decrypted_number));
            }
            // only add possible options
            // todo: memory saving pls
            if (possible)
            {
                XORDecrypted this_decrypt = {decrypted_str, test_delimiter, test_char_length, test_key, test_base};
                calculate_score(this_decrypt);
                // when not filled yet
                if (m_amount == -1 || m_decryptions.size() < m_amount)
                    m_decryptions.push_back(this_decrypt);
                else
                    // when space is rare, overwrite if good enough
                    for (std::vector<XORDecrypted>::iterator ptr = m_decryptions.begin(); ptr < m_decryptions.end(); ptr++)
                        if (*ptr < this_decrypt)
                        {
                            *ptr = this_decrypt;
                            break;
                        }
            }
        }
    }
}

void XORDecrypt::calculate_score(XORDecrypted &decrypt)
{
    // todo: maybe Trie good?
    decrypt.score = 0;
    for (char character : decrypt.decrypted_str)
    {
        // for each printable character
        if (character >= ' ' && character <= '~')
            decrypt.score += 1;
        else
            decrypt.score -= 1;
        // extra points for "good" characters
        if (character >= 'A' && character <= 'Z')
            decrypt.score += 2;
        else if (character >= 'a' && character <= 'z')
            decrypt.score += 2;
        else if (character == ' ')
            decrypt.score += 1;
    }
    decrypt.score /= decrypt.base;

    if (decrypt.key == 0)
        decrypt.score *= 2;
}

void XORDecrypt::create_decryptions()
{
    // test separating encrypted numbers with all possible delimiters
    for (char test_delimiter : m_possible_delimiters)
    {
        std::vector<std::string> encrypted_numbers = cut_cipher_with_delimiter(test_delimiter);
        // todo: not only the first one please
        try_decrypt(encrypted_numbers, test_delimiter, encrypted_numbers[0].size());
    }

    // test separating encrypted numbers with different number lengths
    for (int test_char_length = 1; test_char_length <= 8; test_char_length++)
    {
        if (m_cipher.size() % test_char_length != 0)
            continue;
        std::vector<std::string> encrypted_numbers = cut_cipher_with_char_length(test_char_length);
        try_decrypt(encrypted_numbers, '\0', test_char_length);
    }
    std::sort(m_decryptions.begin(), m_decryptions.end());
}

// todo: better names for everything
// todo: sorting missing
