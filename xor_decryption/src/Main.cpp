#include <iostream>
#include <vector>
#include <math.h>

#include "Decrypted.h"

int character_to_int(char character, int check_base = 36, bool error = true)
{
    // convert digit in base <chack_base> to int
    int digit = check_base;
    if (character >= '0' && character <= '9')
        digit = character - '0';
    else if (character >= 'a' && character <= 'z')
        digit = character - 'a' + 10;
    else if (character >= 'A' && character <= 'Z')
        digit = character - 'A' + 10;

    if (error)
    {
        // check if character is valid in check_base
        if (digit >= check_base)
        {
            std::string error;
            error.push_back(character);
            error.append(" not supported with base ");
            error.append(std::to_string(static_cast<int>(check_base)));
            throw std::runtime_error(error);
        }
    }
    else
        return -1;

    return digit;
}

int decrypt_number(std::string &digit_str, int base, int key = 0)
{
    // convert multiple digits in base <base> to int
    int result_char_code = 0;
    for (char character : digit_str)
        result_char_code = base * result_char_code + character_to_int(character, base, true);

    // xor decryption and convert to ASCII char
    return static_cast<char>(result_char_code ^ key);
}

std::string decrypt_text(std::string &str, int base, int key = 0, char delimiter = ' ', int digit_length = 0)
{
    // convert to ints and then to ASCII chars
    std::string result_str = "";
    for (std::string number : get_numbers(str, delimiter, digit_length))
        result_str.push_back(decrypt_number(number, base, key));
    return result_str;
}

std::vector<std::string> get_numbers(std::string &str, char delimiter = ' ', int digit_length = 0)
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

char get_highest_character(std::string &str)
{
    int highest_character_int = 0;
    for (char character : str)
    {
        if (character_to_int(character, 36, false) > highest_character_int)
            highest_character_int = character;
    }
    return highest_character_int;
}

int get_longest_number_length(std::vector<std::string> &numbers)
{
    int longest_number_length = 0;
    for (std::string number : numbers)
    {
        if (number.size() > longest_number_length)
            longest_number_length = number.size();
    }
}

// todo: remove str
std::vector<int> get_possible_bases(std::vector<std::string> &numbers, std::string str, char delimiter = ' ')
{
    // when the character representing the highest number is '9', base 10 is the "smallest" possible base
    int smallest_possible_base = get_highest_character(str) + 1;
    int longest_number_length = get_longest_number_length(numbers);
    for (int test_base = smallest_possible_base; test_base < 37; test_base++)
    {
    }
}

int main1()
{
    // get input
    std::string input, key_str, base_str;
    std::cout << "input to decrypt: ";
    getline(std::cin, input);
    std::cout << "base: ";
    getline(std::cin, base_str);
    std::cout << "input xor key: ";
    getline(std::cin, key_str);

    int base = stoi(base_str);
    int key = stoi(key_str);

    std::cout << decrypt_text(input, base, key, ' ', 0) << std::endl;
}

int main()
{
    get_possible_bases("1 43 328965 4 2389");
}

// what about ugly input?
