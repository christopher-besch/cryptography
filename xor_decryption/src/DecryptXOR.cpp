#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

#include "Decrypted.h"

std::vector<std::string> get_encrypted_numbers(std::string &str, char delimiter = ' ', int digit_length = 0)
{
    std::vector<std::string> numbers;

    if (delimiter == 0)
    {
        // cut with digit_length
        for (int idx = 0; idx < str.size(); idx += digit_length)
            numbers.push_back(str.substr(idx, digit_length));
    }
    else
    {
        // cut at delimiter
        size_t previous = 0;
        size_t current = str.find(delimiter);
        while (current != std::string::npos)
        {
            std::string substring = str.substr(previous, current - previous);
            numbers.push_back(substring);

            previous = current + 1;
            current = str.find(delimiter, previous);
        }
        std::string substring = str.substr(previous, current - previous);
        numbers.push_back(substring);
    }

    return numbers;
}

int decrypt_character_to_int(char character, int check_base = 36, bool error = true)
{
    // convert digit in base <check_base> to int
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
        if (error)
        {
            std::string error;
            error.push_back(character);
            error.append(" not supported with base ");
            error.append(std::to_string(static_cast<int>(check_base)));
            throw std::runtime_error(error);
        }
        else
            return -1;
    }

    return digit;
}

long long decrypt_number(std::string &digit_str, int base, int key = 0)
{
    // convert multiple digits in base <base> to int
    long long result_char_code = 0;
    for (char character : digit_str)
        result_char_code = base * result_char_code + decrypt_character_to_int(character, base, true);

    // xor decryption and convert to ASCII char
    return result_char_code ^ key;
}

std::vector<int> get_possible_bases(std::vector<std::string> &encrypted_numbers, char highest_character)
{
    std::vector<int> possible_bases;

    // e.g. when the character representing the highest number is '9', base 10 is the "smallest" possible base
    for (int test_base = highest_character + 1; test_base < 37; test_base++)
    {
        bool possible = true;
        for (std::string encrypted_number : encrypted_numbers)
        {
            // todo: test other keys
            long long x = decrypt_number(encrypted_number, test_base, 0) > 255;
            if (decrypt_number(encrypted_number, test_base, 0) > 255)
            {
                possible = false;
                break;
            }
        }
        if (possible)
            possible_bases.push_back(test_base);
    }
    return possible_bases;
}

// todo: amount of requested matches
std::vector<XORDecrypted> get_decryptions(std::string &cipher)
{
    // all characters in the cipher that can't be decrypted
    std::vector<char> possible_delimiters;

    // the highest decryptable character defines the lower limit of the base
    char highest_character = 0;
    for (char character : cipher)
    {
        char decrypted_character = decrypt_character_to_int(character, 36, false);
        if (decrypted_character == -1 && std::find(possible_delimiters.begin(), possible_delimiters.end(), character) == possible_delimiters.end())
            possible_delimiters.push_back(character);

        else if (decrypted_character > highest_character)
            highest_character = decrypted_character;
    }

    for (char delimiter : possible_delimiters)
    {
        // todo: find length
        // cut into encrypted numbers
        std::vector<std::string> encrypted_numbers = get_encrypted_numbers(cipher, ' ', 0);

        std::vector<int> possible_bases = get_possible_bases(encrypted_numbers, highest_character);
        std::cout << "";
    }

    // go through possible bases
}

int main()
{
    std::string str = "1110100 1100101 1110011 1110100";
    get_decryptions(str);
}
