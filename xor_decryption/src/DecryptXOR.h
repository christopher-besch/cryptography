#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include <Utils.h>

#include "Decrypted.h"

int character_to_int(char character, int check_base = 36, bool error = true);

void remove_unsupported_characters(std::string &str);

std::vector<std::string> get_encrypted_numbers(std::string &str, char delimiter, int digit_length);

long long decrypt_number(std::string digit_str, int base, int key = 0);

void try_decrypt(std::vector<XORDecrypted> &results, std::vector<std::string> &encrypted_numbers, char test_delimiter, int test_char_length, int highest_character);

std::vector<XORDecrypted> get_decryptions(std::string cipher);
