#include <iostream>

int char_to_int(char character, int check_base = 36)
{
    int digit = check_base;
    if (character >= '0' && character <= '9')
        digit = character - '0';
    else if (character >= 'a' && character <= 'z')
        digit = character - 'a';
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

char decrypt_digit(std::string digit_str, int base, int key = 0)
{
    int result_char_code = 0;

    for (char character : digit_str)
    {
        result_char_code = base * result_char_code + char_to_int(character, base);
    }

    return static_cast<char>(result_char_code ^ key);
}

std::string decrypt_text(std::string str, int base, char delimiter = ' ', int key = 0)
{
    std::string result_str = "";
    std::string last_digit_str = "";
    for (int idx = 0; idx < str.size(); ++idx)
    {
        if (str[idx] == delimiter)
        {
            result_str.push_back(decrypt_digit(last_digit_str, base, key));
            last_digit_str = "";
        }
        else
        {
            last_digit_str.push_back(str[idx]);
        }
    }
    result_str.push_back(decrypt_digit(last_digit_str, base, key));

    decrypt_digit("d", 10, key = 20);
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
        << decrypt_text(input, base, ' ', key) << std::endl;
}

// todo: make lowercase
