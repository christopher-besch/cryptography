#include <string>
#include <iostream>
#include <fstream>
#include <cmath>

#include "Console.h"
#include "xor/XORDecrypt.h"
#include "transform/TransformDecrypt.h"
#include "goodness/LibrarySearch.h"

int main(int argc, char *argv[])
{
    // todo: read config from json
    ConsoleArguments console_arguments;
    console_arguments.add_optional({"-d", "--delim"}, 1, -1);
    console_arguments.add_optional({"-l", "--len"}, 1, -1);
    console_arguments.add_optional({"-b", "--base"}, 1, -1);
    console_arguments.add_optional({"-k", "--key"}, 1, -1);
    console_arguments.add_optional({"-t", "--transform"}, 1, -1);
    console_arguments.add_optional({"-a", "--amount"}, 1, 1);
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
    // amount of requested decryptions
    int decryptions_amount = 5;
    if (console_arguments["-a"])
        decryptions_amount = checked_stoi(console_arguments["-a"].get_arguments()[0]);

    LibrarySearch dictionary(get_virtual_cwd(console_arguments[0]) + "resources" + file_slash + "words.txt");
    std::vector<const Decrypted *> decryptions;

    // xor
    XORDecryptor xor_decryptor(input, dictionary);
    // load requests
    for (const char *delimiter : console_arguments["-d"].get_arguments())
        xor_decryptor.add_requested_delimiter(delimiter[0]);
    for (const char *length : console_arguments["-l"].get_arguments())
        xor_decryptor.add_requested_char_length(checked_stoi(length));
    for (const char *base : console_arguments["-b"].get_arguments())
        xor_decryptor.add_requested_base(checked_stoi(base));
    for (const char *key : console_arguments["-k"].get_arguments())
        xor_decryptor.add_requested_key(checked_stoi(key));
    // perform decryption
    xor_decryptor.create_decryptions(decryptions_amount);
    // save decryptions
    for (const XORDecrypted &decrypted : xor_decryptor.get_decryptions())
        decryptions.push_back(static_cast<const Decrypted *>(&decrypted));

    // transform
    TransformDecryptor transform_decryptor(input, dictionary);
    for (const char *key : console_arguments["-k"].get_arguments())
        transform_decryptor.add_requested_key(checked_stoi(key));
    for (const char *key : console_arguments["-t"].get_arguments())
        transform_decryptor.add_requested_transformation_types(key);
    // perform decryption
    transform_decryptor.create_decryptions(decryptions_amount);
    // save decryptions
    for (const TransformDecrypted &decrypted : transform_decryptor.get_decryptions())
        decryptions.push_back(static_cast<const Decrypted *>(&decrypted));

    std::sort(decryptions.begin(), decryptions.end(), [](const Decrypted *a, const Decrypted *b) { return *a < *b; });
    // todo: make list of results smaller

    for (int idx = 0; idx < std::min(static_cast<int>(decryptions.size()), decryptions_amount); ++idx)
        std::cout << (*decryptions[idx]).get_report() << std::endl;

    // print
    for (const Decrypted *decrypted : decryptions)
        std::cout << (*decrypted).get_report() << std::endl;

    return 0;
}

// todo: remove really bad results + can't find decryption (e.g. because invalid)
// todo: add help page
// todo: Üöäß
// todo: reverse xor?
