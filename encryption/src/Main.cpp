#include <Console.h>

#include "xor/XOREncrypt.h"

int main(int argc, char *argv[])
{
    ConsoleArguments console_arguments;
    console_arguments.add_optional({"-b", "--base"}, 1, 1);
    console_arguments.add_optional({"-k", "--key"}, 1, 1);
    console_arguments.add_optional({"-d", "--delim"}, 0, 1);
    console_arguments.add_bool({"-0"});
    console_arguments.load_arguments(argc, argv);

    // todo: read from file
    std::string input;
    if (console_arguments.other_size() >= 2)
        input = console_arguments[1];
    else
    {
        std::cout << "Input Text: ";
        std::getline(std::cin, input);
    }

    // todo: read from json
    // select delimiter
    char delimiter = '\0';
    if (console_arguments["-d"])
    {
        if (console_arguments["-d"].get_arguments().empty())
            // default
            delimiter = ' ';
        else
            delimiter = console_arguments["-d"].get_arguments()[0][0];
    }

    // should leading 0s be added -> all char-digits are of same length
    bool add_0 = console_arguments["-0"];

    // key provided?
    int input_key = -1;
    if (console_arguments["-k"])
    {
        input_key = checked_stoi(console_arguments["-k"].get_arguments()[0]);
        if (input_key < 0 || input_key > 255)
            raise_error("The provided key " << input_key << " is invalid!");
    }

    // base provided?
    int input_base = -1;
    if (console_arguments["-b"])
    {
        input_base = checked_stoi(console_arguments["-b"].get_arguments()[0]);
        if (input_base < 2 || input_base > 36)
            raise_error("The provided base " << input_base << " is invalid!");
    }

    // create encryptions
    std::vector<XOREncrypted> encryptions;
    if (input_key != -1)
    {
        if (input_base != -1)
            encryptions.push_back(encrypt_text(input, input_base, input_key, delimiter, add_0));
        // go thorugh every base
        else
            for (int base = 36; base >= 2; base--)
                encryptions.push_back(encrypt_text(input, base, input_key, delimiter, add_0));
    }
    // go through every key
    else
        for (int key = 255; key >= 0; key--)
        {
            if (input_base != -1)
                encryptions.push_back(encrypt_text(input, input_base, key, delimiter, add_0));
            // go thorugh every base
            else
                for (int base = 36; base >= 2; base--)
                    encryptions.push_back(encrypt_text(input, base, key, delimiter, add_0));
        }

    // print encryptions
    std::cout << XOREncrypted::get_header() << std::endl;
    for (XOREncrypted &encrypt : encryptions)
        std::cout << encrypt << std::endl;
}

// todo: add help page
