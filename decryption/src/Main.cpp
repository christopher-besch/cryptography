#include <string>
#include <iostream>
#include <fstream>

#include "Console.h"
#include "xor/XORDecrypt.h"
#include "transform/TransformDecrypt.h"
#include "goodness/LibrarySearch.h"

// only holding up to <MAX_BEST_DECRYPTIONS> decryptions before overwriting the worst decryption
#define MAX_BEST_DECRYPTIONS 20
// when the score of a certain decryption is at least ((1 - <MAX_ALLOWED_DEVIATION>) * <score of best decryption>), it gets printed as well with decryptions_amount < 0
#define MAX_ALLOWED_DEVIATION 0.05f

// should this algorithm be used?
bool is_to_test_algo(const std::string &algo, const ConsoleArguments &console_arguments)
{
    const std::vector<const char *> algos = console_arguments["-a"].get_arguments();
    return !console_arguments["-a"] || std::find(algos.begin(), algos.end(), algo) != algos.end();
}

// decrypt a single string with all supported algorithms and return reports for each decryption
std::string do_decryptions(const std::string &str, const LibrarySearch &dictionary, const ConsoleArguments &console_arguments)
{
    // amount of requested decryptions
    int hold_amount = -1;
    if (console_arguments["-m"])
        hold_amount = checked_stoi(console_arguments["-m"].get_arguments()[0]);
    bool print_best_only = false;
    if (hold_amount < 0)
    {
        hold_amount = MAX_BEST_DECRYPTIONS;
        print_best_only = true;
    }

    std::string result;
    // reserve memory (only approximation)
    result.reserve(str.size());
    std::vector<const Decrypted *> decryptions;

    XORDecryptor xor_decryptor(str, dictionary);
    TransformDecryptor transform_decryptor(str, dictionary);
    // only use requested algorithms
    if (is_to_test_algo("xor", console_arguments))
    {
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
        xor_decryptor.create_decryptions(hold_amount);
        // save decryptions
        for (XORDecrypted &decrypted : xor_decryptor.get_decryptions())
        {
            decrypted.create_report();
            decryptions.push_back(static_cast<const Decrypted *>(&decrypted));
        }
    }
    if (is_to_test_algo("transform", console_arguments))
    {
        // load requests
        for (const char *key : console_arguments["-k"].get_arguments())
            transform_decryptor.add_requested_key(checked_stoi(key));
        for (const char *key : console_arguments["-t"].get_arguments())
            transform_decryptor.add_requested_transformation_type(key);
        // perform decryption
        transform_decryptor.create_decryptions(hold_amount);
        // save decryptions
        for (TransformDecrypted &decrypted : transform_decryptor.get_decryptions())
        {
            decrypted.create_report();
            decryptions.push_back(static_cast<const Decrypted *>(&decrypted));
        }
    }

    if (decryptions.empty())
        raise_error("Failed to create any decryptions for '" << str << "'!");

    if (print_best_only)
    {
        // best results come first
        std::sort(decryptions.begin(), decryptions.end(), [](const Decrypted *a, const Decrypted *b) { return *a > *b; });
        // only print all best results
        float best_score = decryptions[0]->score;
        bool first_worse_found = false;
        for (const Decrypted *decryption : decryptions)
        {
            // add seperating line between best and worse decryptions
            if (decryption->score != best_score && !first_worse_found)
            {
                first_worse_found = true;
                result.append("------------------------------\n");
            }
            // first disallowed decryption ends print
            if (decryption->score != best_score && decryption->score < (1 - MAX_ALLOWED_DEVIATION) * best_score)
                break;
            result.append(decryption->report);
            result.push_back('\n');
        }
    }
    // print certain amount of decrypts
    else
    {
        // best results come last
        std::sort(decryptions.begin(), decryptions.end(), [](const Decrypted *a, const Decrypted *b) { return *a < *b; });

        int start_idx = decryptions.size() - hold_amount;
        // when start idx is negative <- decryptions_amount is too big
        if (!hold_amount || start_idx < 0)
            start_idx = 0;

        // only print as many as requested -> print until last one
        for (int idx = start_idx; idx < decryptions.size(); ++idx)
        {
            result.append(decryptions[idx]->report);
            result.push_back('\n');
        }
    }
#ifdef DEBUG
    if (result.empty())
        raise_error("Failed to sort decryptions for '" << str << "'!");
#endif
    return result;
}

int main(int argc, char *argv[])
{
    ConsoleArguments console_arguments;
    console_arguments.add_optional({"-d", "--delim"}, 1, -1);
    console_arguments.add_optional({"-l", "--len"}, 1, -1);
    console_arguments.add_optional({"-b", "--base"}, 1, -1);
    console_arguments.add_optional({"-k", "--key"}, 1, -1);
    console_arguments.add_optional({"-t", "--transform"}, 1, -1);
    console_arguments.add_optional({"-a", "--algorithm"}, 1, -1);
    console_arguments.add_optional({"-m", "--amount"}, 1, 1);
    console_arguments.add_optional({"-f", "--file"}, 2, 2);
    console_arguments.load_arguments(argc, argv);

    // algorithms valid?
    for (const std::string &algo : console_arguments["-a"].get_arguments())
        if (algo != "xor" && algo != "transform")
            raise_error("Invalid algorithm '" << algo << "'!");

    // load dictionary
    std::cerr << "loading dictionaries" << std::endl;
    LibrarySearch dictionary;
    // from <cwd decryption>/resources/*
#ifdef IDE
    dictionary.load_file(std::string("decryption") + file_slash + "resources" + file_slash + "english.dic");
    dictionary.load_file(std::string("decryption") + file_slash + "resources" + file_slash + "german1.dic");
    dictionary.load_file(std::string("decryption") + file_slash + "resources" + file_slash + "german2.dic");
    dictionary.load_file(std::string("decryption") + file_slash + "resources" + file_slash + "user_dict.dic");
    // from <executable directory>/resources/*
    // todo: doesn't work when accessed thorugh PATH
#else
    dictionary.load_file(get_exec_dir(console_arguments[0]) + "resources" + file_slash + "english.dic");
    dictionary.load_file(get_exec_dir(console_arguments[0]) + "resources" + file_slash + "german1.dic");
    dictionary.load_file(get_exec_dir(console_arguments[0]) + "resources" + file_slash + "german2.dic");
    dictionary.load_file(get_exec_dir(console_arguments[0]) + "resources" + file_slash + "user_dict.dic");
#endif

    std::cerr << "starting decryption" << std::endl;
    // read each line from file as one cipher
    if (console_arguments["-f"])
    {
        std::fstream in_file(console_arguments["-f"].get_arguments()[0], std::ios::in);
        std::fstream out_file(console_arguments["-f"].get_arguments()[1], std::ios::out);

        if (!in_file)
            raise_error("Can't open file '" << console_arguments["-f"].get_arguments()[0] << "'!");
        if (!out_file)
            raise_error("Can't open file '" << console_arguments["-f"].get_arguments()[1] << "'!");

        int line = 1;
        bool empty = true;
        for (std::string cipher; std::getline(in_file, cipher); ++line)
        {
            // perform checks
            if (cipher.empty())
                continue;
            for (char character : cipher)
                if (!(character >= ' ' && character <= '~'))
                    raise_error("Unsupported character '" << character << "' found in line " << line << "!");
            // create and store decryptions
            out_file << "# line " << line << ": " << cipher << std::endl;
            std::string decryption_result = do_decryptions(cipher, dictionary, console_arguments);
            out_file << decryption_result << std::endl;
            std::cerr << "line " << line << " decrypted" << std::endl;
            empty = false;
        }
        in_file.close();
        out_file.close();
        if (empty)
            raise_error("'" << console_arguments["-f"].get_arguments()[0] << "' is empty!");
    }
    // only one input cipher from the console
    else
    {
        // get cipher
        std::string input;
        if (console_arguments.other_size() >= 2)
            input = console_arguments[1];
        else
        {
            std::cout << "Input Cipher: ";
            std::getline(std::cin, input);
        }
        for (char character : input)
            if (!(character >= ' ' && character <= '~'))
                raise_error("Unsupported character '" << character << "' found!");

        // decrypt cipher and print results
        std::string decryption_result = do_decryptions(input, dictionary, console_arguments);
        std::cout << "------------------------------" << std::endl;
        std::cout << decryption_result << std::endl;
    }

    return 0;
}
// todo: add help page
