#include <string>
#include <iostream>

inline void make_lower_case(std::string &str)
{
    for (char &character : str)
        if (character >= 'A' && character <= 'Z')
            character += 'a' - 'A';
}

void caesar_encrypt(std::string &str, int key)
{
    make_lower_case(str);
    for (char &character : str)
    {
        // spaces don't get substituted
        if (character >= 'a' && character <= 'z')
        {
            int character_id = character - 'a';
            int moved_id = (character_id + key) % 26;
            character = 'a' + moved_id;
        }
    }
}

// int main()
// {
//     std::string text = "If he had anything confidential to say, he wrote it in cipher, that is, by so changing the order of the letters of the alphabet, that not a word could be made out.";
//     caesar_encrypt(text, 7);
//     std::cout << text << std::endl;
//     return 0;
// }

// output:
// pm ol ohk hufaopun jvumpkluaphs av zhf, ol dyval pa pu jpwoly, aoha pz, if zv johunpun aol vykly vm aol slaalyz vm aol hswohila, aoha uva h dvyk jvbsk il thkl vba.
