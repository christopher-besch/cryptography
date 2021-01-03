#include "xor/XOREncrypt.h"

int main()
{
    // todo: read from json
    // todo: add checks
    // get input
    std::string input, key_str;
    std::cout << "input to encrypt: ";
    std::getline(std::cin, input);
    std::cout << "input xor key: ";
    std::getline(std::cin, key_str);

    int key = std::stoi(key_str);

    // from base 36 to base 2
    for (int base = 36; base > 1; --base)
    {
        std::cout << base << ": " << encrypt_text(input, base, key, ' ', false) << std::endl;
    }
}
