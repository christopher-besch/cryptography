#include <string>
#include <iostream>
#include <fstream>

#include <Console.h>
#include "xor/XORDecrypt.h"
#include "goodness/LibrarySearch.h"

int main(int argc, char *argv[])
{
    // todo: read config from json
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

    LibrarySearch dictionary("resources/words.txt");
    XORDecrypt decrypts(input, dictionary);
    // load requests
    for (const char *delimiter : console_arguments["-d"].get_arguments())
        decrypts.add_requested_delimiter(delimiter[0]);
    for (const char *length : console_arguments["-l"].get_arguments())
        decrypts.add_requested_char_length(checked_stoi(length));
    for (const char *base : console_arguments["-b"].get_arguments())
        decrypts.add_requested_base(checked_stoi(base));
    for (const char *key : console_arguments["-k"].get_arguments())
        decrypts.add_requested_key(checked_stoi(key));

    decrypts.create_decryptions(5);

    // print
    std::cout << XORDecrypted::get_header() << std::endl;
    for (const XORDecrypted &decrypt : decrypts.get_decryptions())
        std::cout << decrypt << std::endl;

    // todo: bad alignment
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

// todo: remove really bad results + can't find decryption (e.g. because invalid)
// todo: add help page
// todo: add good readme
