#include <Console.h>

#include "xor/XOREncrypt.h"
#include "transpose/TransposeEncrypt.h"

// create and print xor encryptions
void do_xor_encryption(const std::string &str, int input_base, int input_key, char delimiter, bool add_0)
{
    std::vector<XOREncrypted> encryptions;
    if (input_key != -1)
    {
        if (input_base != -1)
            encryptions.push_back(xor_encrypt(str, input_base, input_key, delimiter, add_0));
        // go thorugh every base
        else
            for (int base = 36; base >= 2; base--)
                encryptions.push_back(xor_encrypt(str, base, input_key, delimiter, add_0));
    }
    // go through every key
    else
        for (int key = 255; key >= 0; key--)
        {
            if (input_base != -1)
                encryptions.push_back(xor_encrypt(str, input_base, key, delimiter, add_0));
            // go thorugh every base
            else
                for (int base = 36; base >= 2; base--)
                    encryptions.push_back(xor_encrypt(str, base, key, delimiter, add_0));
        }
    if (encryptions.empty())
        raise_error("Error while creating encryptions!");

    // print encryptions
    std::cout << "base\tkey\tresult" << std::endl;
    for (XOREncrypted &encrypt : encryptions)
        std::cout << encrypt.get_base() << "\t" << encrypt.get_key() << "\t" << encrypt.get_encrypted_str() << std::endl;
}

// todo: make one function out of these
void do_plow_encryption(const std::string &str, int input_key)
{
    std::vector<TransposeEncrypted> encryptions;
    if (input_key != -1)
    {
        if (input_key < 1)
            raise_error("Unsupported key: " << input_key);
        encryptions.push_back(plow_encrypt(str, input_key));
    }
    // go through every key
    else
        for (int key = str.size(); key >= 1; --key)
            encryptions.push_back(plow_encrypt(str, key));

    if (encryptions.empty())
        raise_error("Error while creating encryptions!");

    // print encryptions
    std::cout << "key\tresult" << std::endl;
    for (TransposeEncrypted &encrypt : encryptions)
        std::cout << encrypt.get_key() << "\t" << encrypt.get_encrypted_str() << std::endl;
}

void do_transpose_encryption(const std::string &str, int input_key)
{
    std::vector<TransposeEncrypted> encryptions;
    if (input_key != -1)
    {
        if (input_key < 1)
            raise_error("Unsupported key: " << input_key);
        encryptions.push_back(transpose_encrypt(str, input_key));
    }
    // go through every key
    else
        for (int key = str.size(); key >= 1; --key)
            encryptions.push_back(transpose_encrypt(str, key));

    if (encryptions.empty())
        raise_error("Error while creating encryptions!");

    // print encryptions
    std::cout << "key\tresult" << std::endl;
    for (TransposeEncrypted &encrypt : encryptions)
        std::cout << encrypt.get_key() << "\t" << encrypt.get_encrypted_str() << std::endl;
}

int main(int argc, char *argv[])
{
    ConsoleArguments console_arguments;
    console_arguments.add_optional({"-b", "--base"}, 1, 1);
    console_arguments.add_optional({"-k", "--key"}, 1, 1);
    console_arguments.add_optional({"-d", "--delim"}, 0, 1);
    console_arguments.add_optional({"-a", "--algorithm"}, 1, 1);
    console_arguments.add_bool({"-0"});
    console_arguments.load_arguments(argc, argv);

    // todo: read from file
    std::string str;
    if (console_arguments.other_size() >= 2)
        str = console_arguments[1];
    else
    {
        std::cout << "Input Text: ";
        std::getline(std::cin, str);
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
    int key = -1;
    if (console_arguments["-k"])
    {
        key = checked_stoi(console_arguments["-k"].get_arguments()[0]);
        if (key < 0 || key > 255)
            raise_error("The provided key " << key << " is invalid!");
    }

    // base provided?
    int base = -1;
    if (console_arguments["-b"])
    {
        base = checked_stoi(console_arguments["-b"].get_arguments()[0]);
        if (base < 2 || base > 36)
            raise_error("The provided base " << base << " is invalid!");
    }

    // algorithm provided?
    std::string algorithm;
    if (console_arguments["-a"])
        algorithm = console_arguments["-a"].get_arguments()[0];
    else
    {
        std::cout << "Input Encryption Algorithm (xor, plow and transpose are supported): ";
        std::getline(std::cin, algorithm);
    }
    make_lower_case(algorithm);

    // do right encryption
    if (algorithm == "xor")
        do_xor_encryption(str, base, key, delimiter, add_0);
    else if (algorithm == "plow")
        do_plow_encryption(str, key);
    else if (algorithm == "transpose")
        do_transpose_encryption(str, key);
    else
        raise_error("'" << algorithm << "' is an unsupported encryption algorithm");
}

// todo: add help page
