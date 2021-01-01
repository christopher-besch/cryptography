#include <string>
#include <iostream>

#include <Console.h>
#include "xor/XORDecrypt.h"

int main(int argc, char *argv[])
{
    // todo: read config from json
    // todo: add force mode only searching for requested
    ConsoleArguments console_arguments;
    console_arguments.add_optional({"-d", "--delim"}, -1);
    console_arguments.add_optional({"-l", "--len"}, -1);
    console_arguments.add_optional({"-b", "--base"}, -1);
    console_arguments.add_optional({"-k", "--key"}, -1);
    console_arguments.load_arguments(argc, argv);

    // todo: read from file
    std::string input;
    if (console_arguments.other_size() >= 2)
        input = console_arguments[1];
    else
    {
        std::cout << "Input Cipher: ";
        std::getline(std::cin, input);
    }

    XORDecrypt decrypts(input);

    for (const char *delimiter : console_arguments["-d"].get_arguments())
        decrypts.add_requested_delimiter(delimiter[0]);
    for (const char *length : console_arguments["-l"].get_arguments())
        decrypts.add_requested_char_length(checked_stoi(length));
    for (const char *base : console_arguments["-b"].get_arguments())
        decrypts.add_requested_base(checked_stoi(base));
    for (const char *key : console_arguments["-k"].get_arguments())
        decrypts.add_requested_key(checked_stoi(key));

    std::cout << "";
}
