#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <unordered_map>
#include <cstring>

#include "Console.h"
#include "Core.h"
#include "Decrypted.h"

int character_to_int(char character, int check_base = 36, bool error = true)
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
            std::stringstream error;
            error << "'" << character << "' not supported with base " << std::to_string(static_cast<int>(check_base));
            throw std::runtime_error(error.str());
        }
        else
            return -1;
    }
    return digit;
}

void remove_unsupported_characters(std::string &str)
{
    for (int idx = str.size() - 1; idx >= 0; idx--)
        if (character_to_int(str[idx], 36, false) == -1)
            str.erase(idx, 1);
}

std::vector<std::string> get_encrypted_numbers(std::string &str, char delimiter, int digit_length)
{
    std::vector<std::string> numbers;

    if (delimiter == '\0')
        // cut with digit_length
        for (int idx = 0; idx < str.size(); idx += digit_length)
            numbers.push_back(str.substr(idx, digit_length));
    else
    {
        // cut at delimiter
        size_t previous = 0;
        size_t current = str.find(delimiter);
        while (current != std::string::npos)
        {
            std::string substring = str.substr(previous, current - previous);
            remove_unsupported_characters(substring);
            if (substring.size() != 0)
                numbers.push_back(substring);

            previous = current + 1;
            current = str.find(delimiter, previous);
        }
        std::string substring = str.substr(previous, current - previous);
        remove_unsupported_characters(substring);
        if (substring.size() != 0)
            numbers.push_back(substring);
    }
    return numbers;
}

long long decrypt_number(std::string digit_str, int base, int key = 0)
{
    // convert multiple digits in base <base> to int
    long long result_char_code = 0;
    for (char character : digit_str)
        result_char_code = base * result_char_code + character_to_int(character, base, true);

    // xor decryption and convert to ASCII char
    return result_char_code ^ key;
}

void try_decrypt(std::vector<XORDecrypted> &results, std::vector<std::string> &encrypted_numbers, char test_delimiter, int test_char_length, int highest_character)
{
    // test possible bases
    // e.g. when the character representing the highest number is '9', base 10 is the "smallest" possible base
    for (int test_base = highest_character + 1; test_base < 37; test_base++)
    {
        // todo: test more keys
        // test possible keys
        for (int test_key = 0; test_key < 256; test_key++)
        {
            bool possible = true;
            std::string decrypted_str = "";
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
            // only add new options
            if (possible && std::find_if(results.begin(), results.end(), [&](XORDecrypted item) { return item.get_text() == decrypted_str; }) == results.end())
            {
                XORDecrypted decrypt = XORDecrypted(decrypted_str, test_delimiter, test_char_length, test_key, test_base);
                results.push_back(decrypt);
            }
        }
    }
}

std::vector<XORDecrypted> get_decryptions(std::string cipher)
{
    // all characters in the cipher that can't be decrypted
    std::vector<char> possible_delimiters;

    // the highest decryptable character defines the lower limit of the base
    int highest_character = 0;
    for (char character : cipher)
    {
        char decrypted_character = character_to_int(character, 36, false);
        if (decrypted_character == -1 && std::find(possible_delimiters.begin(), possible_delimiters.end(), character) == possible_delimiters.end())
            possible_delimiters.push_back(character);

        else if (decrypted_character > highest_character)
            highest_character = decrypted_character;
    }

    // test separating numbers with all possible delimiters
    std::vector<XORDecrypted> results;
    for (char test_delimiter : possible_delimiters)
    {
        std::vector<std::string> encrypted_numbers = get_encrypted_numbers(cipher, test_delimiter, 0);
        // todo: not only the first one please
        try_decrypt(results, encrypted_numbers, test_delimiter, encrypted_numbers[0].size(), highest_character);
    }

    // test separating numbers with different number lengths
    remove_unsupported_characters(cipher);
    for (int char_length = 1; char_length <= 8; char_length++)
    {
        if (cipher.size() % char_length != 0)
            continue;
        std::vector<std::string> encrypted_numbers = get_encrypted_numbers(cipher, '\0', char_length);
        try_decrypt(results, encrypted_numbers, '\0', char_length, highest_character);
    }

    std::sort(results.begin(), results.end());
    return results;
}

int main(int argc, char *argv[])
{
    ConsoleArguments console_arguments;
    console_arguments.add_optional({"-b", "--base"});
    console_arguments.add_bool({"-c"});
    console_arguments.load_arguments(argc, argv);

    std::string input;
    if (argc > 1)
        input = argv[argc - 1];
    else
    {
        std::cout << "Input Cipher: ";
        std::getline(std::cin, input);
    }

    std::vector<XORDecrypted> options = get_decryptions(input);

    for (int idx = std::max(static_cast<int>(options.size() - 5), 0); idx < options.size(); idx++)
        std::cout << options[idx].get_score() << " '" << options[idx].get_delimiter() << "' " << options[idx].get_char_length() << " " << options[idx].get_base() << " " << options[idx].get_key() << " " << options[idx].get_text() << std::endl;
}
