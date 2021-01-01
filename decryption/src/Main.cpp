#include <string>
#include <iostream>

#include <Console.h>
#include <DecryptXOR.h>

int main(int argc, char *argv[])
{
    ConsoleArguments console_arguments;
    console_arguments.add_optional({"-d", "--delim"}, -1);
    console_arguments.add_optional({"-l", "--len"}, -1);
    console_arguments.add_optional({"-b", "--base"}, -1);
    console_arguments.add_optional({"-k", "--key"}, -1);
    console_arguments.load_arguments(argc, argv);

    std::string input;
    if (console_arguments.other_size() >= 1)
        input = console_arguments[0];
    else
    {
        std::cout << "Input Cipher: ";
        std::getline(std::cin, input);
    }

    std::vector<XORDecrypted> options = try_xor_decryption(input);

    for (int idx = std::max(static_cast<int>(options.size() - 5), 0); idx < options.size(); idx++)
        std::cout << options[idx].get_score() << " '" << options[idx].get_delimiter() << "' " << options[idx].get_char_length() << " " << options[idx].get_base() << " " << options[idx].get_key() << " " << options[idx].get_text() << std::endl;
}
