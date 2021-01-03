#include "XOREncrypt.h"

// use xor on single number and encode to base <base>
std::string encrypt_number(int number, int base, int key, int min_length)
{
#ifdef DEBUG
    if (min_length != 1 && min_length < get_encrypted_char_length(base))
        raise_error("min_length " << min_length << " is too small for base " << base << "!");
#endif
    char digit_chars[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
                          'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    int char_code = number ^ key;

    std::string result_digits;
    // going through every digit
    // add more 0 if initial char_code is too small for min_length
    for (int idx = min_length - 1; idx >= 0 || char_code; idx--)
    {
        char this_digit = digit_chars[char_code % base];
        result_digits.push_back(this_digit);

        char_code = char_code / base;
    }
    reverse(result_digits.begin(), result_digits.end());
    return result_digits;
}

// encrypt text with xor to string of numbers in base <base> with delimiter if requested
XOREncrypted encrypt_text(std::string str, int base, int key, char delimiter, bool add_0)
{
    // add 0 if requested
    int min_length = 1;
    if (add_0)
        min_length = get_encrypted_char_length(base);

    std::string encrypted_str;
    for (char chr : str)
    {
        encrypted_str.append(encrypt_number(static_cast<int>(chr), base, key, min_length));
        if (delimiter != '\0')
            encrypted_str.push_back(delimiter);
    }
    return {encrypted_str, base, key};
}
