#include <string>
#include <iostream>

#include <Console.h>
#include <DecryptXOR.h>

int main(int argc, char *argv[])
{
    ConsoleArguments console_arguments;
    console_arguments.add_optional({"-b", "--base"});
    console_arguments.add_bool({"-c"});
    console_arguments.load_arguments(argc, argv);

    std::string input;
    if (argc > 1)
        input = argv[argc - 1];
    else
    {
        std::cout << "Input Cipher: ";
        std::getline(std::cin, input);
    }

    std::vector<XORDecrypted> options = get_decryptions(input);

    for (int idx = std::max(static_cast<int>(options.size() - 5), 0); idx < options.size(); idx++)
        std::cout << options[idx].get_score() << " '" << options[idx].get_delimiter() << "' " << options[idx].get_char_length() << " " << options[idx].get_base() << " " << options[idx].get_key() << " " << options[idx].get_text() << std::endl;
}
