#include "XORDecrypt.h"

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>

#include <Utils.h>

int XORDecryptor::character_to_int(char character, int check_base, bool error)
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
            raise_error("'" << character << "' not supported with base '" << check_base << "'!");
    }
    return digit;
}

void XORDecryptor::preprocess()
{
    m_smallest_base = 0;
    for (char character : m_cipher)
    {
        // when a character causes character_to_int to fail, it is a possible delimiter
        char decrypted_character = character_to_int(character, 36, false);
        // add each only once
        if (decrypted_character == -1 && std::find(m_possible_delimiters.begin(), m_possible_delimiters.end(), character) == m_possible_delimiters.end())
            m_possible_delimiters.push_back(character);
        // e.g. when the character representing the highest number is '9', base 10 is the "smallest" possible base
        else if (decrypted_character + 1 > m_smallest_base)
            m_smallest_base = decrypted_character + 1;
    }
    // remove everything but the encrypted characters itself
    remove_chars(m_cipher_chars_only, m_possible_delimiters);
}

std::vector<std::string> XORDecryptor::cut_cipher_with_char_length(int char_length)
{
    std::vector<std::string> encrypted_numbers;
    encrypted_numbers.reserve(m_cipher_chars_only.size() / char_length);
    for (int idx = 0; idx < m_cipher_chars_only.size(); idx += char_length)
        encrypted_numbers.push_back(m_cipher_chars_only.substr(idx, char_length));
    return encrypted_numbers;
}

std::vector<std::string> XORDecryptor::cut_cipher_with_delimiter(char delimiter)
{
    std::vector<std::string> encrypted_numbers;
    std::stringstream ss_cipher(m_cipher);

    for (std::string buffer; std::getline(ss_cipher, buffer, delimiter);)
    {
        // remove everything but the characters itself
        remove_chars(buffer, m_possible_delimiters);
        // doubled delimiters aren't a problem
        if (!buffer.empty())
        {
            encrypted_numbers.push_back(buffer);
            // when this string is too big to represent a number
            if (buffer.size() > 8)
                return {};
        }
    }
    return encrypted_numbers;
}

long long XORDecryptor::decrypt_number(std::string encrypted_number, int base, int key)
{
    long long result_char_code = 0;
    for (char character : encrypted_number)
        result_char_code = base * result_char_code + character_to_int(character, base, true);

    // xor decryption
    return result_char_code ^ key;
}

void XORDecryptor::decrypt(std::vector<std::string> &encrypted_numbers, XORDecrypted &template_decrypt)
{
    bool possible = true;
    std::string decrypted_str = "";
    // decrypt each number
    for (std::string encrypted_number : encrypted_numbers)
    {
        // is decodable with extended ASCII?
        long long decrypted_number = decrypt_number(encrypted_number, template_decrypt.base, template_decrypt.key);
        if (decrypted_number < 0 || decrypted_number > 255)
        {
            possible = false;
            break;
        }
        decrypted_str.push_back(decrypted_number);
    }
    if (possible)
    {
        // save decryption
        template_decrypt.decrypted_str = decrypted_str;
        template_decrypt.score = m_dictionary.get_score(decrypted_str);
        // when not filled yet
        if (!m_amount || m_decryptions.size() < m_amount)
            m_decryptions.push_back(template_decrypt);
        else
            // when space is rare, overwrite if good enough
            for (std::vector<XORDecrypted>::iterator ptr = m_decryptions.begin(); ptr < m_decryptions.end(); ptr++)
                if (*ptr < template_decrypt)
                {
                    *ptr = template_decrypt;
                    break;
                }
    }
}

void XORDecryptor::test_decryptions(std::vector<std::string> &encrypted_numbers, XORDecrypted &template_decrypt)
{
    // test possible bases
    for (int test_base = m_smallest_base; test_base < 37; test_base++)
    {
        XORDecrypted this_decrypt;
        this_decrypt.base = test_base;
        if (is_to_test_base(test_base))
        {
            template_decrypt.base = test_base;
            // test possible keys
            for (int test_key = 0; test_key < 256; test_key++)
                if (is_to_test_key(test_key))
                {
                    template_decrypt.key = test_key;
                    decrypt(encrypted_numbers, template_decrypt);
                }
        }
    }
}

void XORDecryptor::create_decryptions(int amount)
{
    m_amount = amount;
    if (m_amount)
        m_decryptions.reserve(m_amount);

    // more and more settings are getting stored in copies of this object
    XORDecrypted template_decrypt;
    // test separating encrypted numbers with all possible delimiters
    for (char test_delimiter : m_possible_delimiters)
        if (is_to_test_delimiter(test_delimiter))
        {
            std::vector<std::string> encrypted_numbers = cut_cipher_with_delimiter(test_delimiter);
            // when unable to find anything or numbers are too long
            if (encrypted_numbers.empty())
                continue;
            template_decrypt.delimiter = test_delimiter;
            template_decrypt.char_length = 0;
            test_decryptions(encrypted_numbers, template_decrypt);
        }

    // test separating encrypted numbers with different char lengths
    for (int test_char_length = 1; test_char_length <= 8; test_char_length++)
        if (is_to_test_char_length(test_char_length))
        {
            // when cipher cannot be cut into equal sized char-digits
            if (m_cipher_chars_only.size() % test_char_length)
                continue;
            std::vector<std::string> encrypted_numbers = cut_cipher_with_char_length(test_char_length);
            template_decrypt.delimiter = '\0';
            template_decrypt.char_length = test_char_length;
            test_decryptions(encrypted_numbers, template_decrypt);
        }
}
