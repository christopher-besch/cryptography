#include <string>
#include <iostream>
#include <fstream>

#include <Console.h>
#include "xor/XORDecrypt.h"
#include "goodness/LibrarySearch.h"

int main(int argc, char *argv[])
{
    // todo: read config from json
    // todo: add force mode only searching for requested
    ConsoleArguments console_arguments;
    console_arguments.add_optional({"-d", "--delim"}, 1, -1);
    console_arguments.add_optional({"-l", "--len"}, 1, -1);
    console_arguments.add_optional({"-b", "--base"}, 1, -1);
    console_arguments.add_optional({"-k", "--key"}, 1, -1);
    console_arguments.add_optional({"-f", "--file"}, 0, 1);
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

    XORDecrypt decrypts(input, -1);
    // load dictionaries and requests
    decrypts.load_dictionary("english.txt");
    for (const char *delimiter : console_arguments["-d"].get_arguments())
        decrypts.add_requested_delimiter(delimiter[0]);
    for (const char *length : console_arguments["-l"].get_arguments())
        decrypts.add_requested_char_length(checked_stoi(length));
    for (const char *base : console_arguments["-b"].get_arguments())
        decrypts.add_requested_base(checked_stoi(base));
    for (const char *key : console_arguments["-k"].get_arguments())
        decrypts.add_requested_key(checked_stoi(key));

    decrypts.create_decryptions();

    // print
    std::cout << XORDecrypted::get_header() << std::endl;
    for (const XORDecrypted &decrypt : decrypts.get_decryptions())
        std::cout << decrypt << std::endl;

    // file output
    if (console_arguments["-f"])
    {
        std::string file_path("output.txt");
        if (console_arguments["-f"].get_arguments().size() != 0)
            file_path = console_arguments["-f"].get_arguments()[0];
        std::ofstream file(file_path);
        file << XORDecrypted::get_header() << std::endl;
        for (const XORDecrypted &decrypt : decrypts.get_decryptions())
            file << decrypt << std::endl;
        file.close();
    }
    return 0;
}

// todo: char-digit seperation with length not working

// 54 39 49 c  3c 3j 47  35 42 39  49 3j 45  3b 3j 3j 38  43 3d 42 1
// base 20
// key 32
// todo: not working
