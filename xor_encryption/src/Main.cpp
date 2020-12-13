#include <iostream>
#include <algorithm>

std::string encrypt_char(char character, int base, int key = 0)
{
    char digit_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                          'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    int char_code = static_cast<int>(character) ^ key;

    // go though every digit
    std::string result_digits;
    while (char_code != 0)
    {
        char this_digit = digit_chars[char_code % base];
        result_digits.push_back(this_digit);

        char_code = char_code / base;
    }
    reverse(result_digits.begin(), result_digits.end());
    return result_digits;
}

std::string encrypt_test(std::string str, int base, char delimiter = '\0', int key = 0)
{
    std::string result;

    for (char chr : str)
    {
        result.append(encrypt_char(chr, base, key));
        if (delimiter != '\0')
            result.push_back(delimiter);
    }
    return result;
}

int main()
{
    // get input
    std::string input, key_str;
    std::cout << "input to encrypt: ";
    getline(std::cin, input);
    std::cout << "input xor key: ";
    getline(std::cin, key_str);

    int key = stoi(key_str);

    // from base 36 to base 2
    for (int base = 36; base > 1; --base)
    {
        std::cout << base << ": " << encrypt_test(input, base, ' ', key) << std::endl;
    }
}
