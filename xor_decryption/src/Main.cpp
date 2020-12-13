#include <iostream>

int character_to_int(char character, int check_base = 36)
{
    // convert digit in base <chack_base> to int
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
        std::string error;
        error.push_back(character);
        error.append(" not supported with base ");
        error.append(std::to_string(static_cast<int>(check_base)));
        throw std::runtime_error(error);
    }
    return digit;
}

int decrypt_number(std::string digit_str, int base, int key = 0)
{
    // convert multiple digits in base <base> to int
    int result_char_code = 0;
    for (char character : digit_str)
        result_char_code = base * result_char_code + character_to_int(character, base);

    // xor decryption and convert to ASCII char
    return static_cast<char>(result_char_code ^ key);
}

std::string decrypt_text(std::string str, int base, int key = 0, char delimiter = ' ', int digit_length = 0)
{
    // extract numbers from string (by delimiter or digit_length) and convert to ints and then to ASCII chars
    std::string result_str = "";
    if (delimiter == 0)
    {
        for (int idx = 0; idx < str.size(); idx += digit_length)
            result_str.push_back(decrypt_number(str.substr(idx, digit_length), base, key));
    }
    else
    {
        // cut at delimiter
        size_t previous = 0;
        size_t current = str.find(delimiter);
        while (current != std::string::npos)
        {
            result_str.push_back(decrypt_number(str.substr(previous, current - previous), base, key));

            previous = current + 1;
            current = str.find(delimiter, previous);
        }
        result_str.push_back(decrypt_number(str.substr(previous, current - previous), base, key));
    }
    return result_str;
}

int main()
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

    // std::cout << base << ": " << convert_str(input, base, ' ', key = key) << std::endl;
    std::cout
        << decrypt_text(input, base, key, ' ', 0) << std::endl;
}

// todo: make lowercase
