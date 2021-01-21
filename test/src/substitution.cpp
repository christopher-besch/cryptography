#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>

#ifdef DEBUG
#define raise_error(msg)                                                                                                  \
    {                                                                                                                     \
        std::cerr << msg << " (in: " << __FILE__ << ":" << __LINE__ << "; in function: " << __func__ << ")" << std::endl; \
        std::exit(EXIT_FAILURE);                                                                                          \
    }
#else
#define raise_error(msg)               \
    {                                  \
        std::cerr << msg << std::endl; \
        std::exit(EXIT_FAILURE);       \
    }
#endif

inline void make_lower_case(std::string &str)
{
    for (char &character : str)
        if (character >= 'A' && character <= 'Z')
            character += 'a' - 'A';
}

std::unordered_map<char, char> create_sub_tab(std::string key)
{
    make_lower_case(key);

    // remove seperators and doubled chars
    for (int idx = key.size() - 1; idx >= 0; idx--)
        if (key[idx] < 'a' || key[idx] > 'z' || std::find(key.begin(), key.begin() + idx, key[idx]) != key.begin() + idx)
            key.erase(idx, 1);

    std::unordered_map<char, char> sub_tab;

    char last_char = 0;
    // fill from key
    for (int idx = 0; idx < 26; ++idx)
    {
        if (idx < key.size())
        {
            sub_tab.insert({'a' + idx, key[idx]});
            last_char = key[idx];
        }
        else
            break;
    }

    // fill after key
    for (int idx = key.size(); idx < 26; ++idx)
    {
        // find first unused char
        while (true)
        {
            bool possible = true;
            for (auto &sub_pair : sub_tab)
                if (sub_pair.second == last_char)
                {
                    possible = false;
                    break;
                }
            if (possible)
                break;
            else
            {
                // rollover
                ++last_char;
                if (last_char > 'z')
                    last_char = 'a';
            }
        }
        sub_tab.insert({'a' + idx, last_char});
    }

    // for (char idx = 'a'; idx < 'z' + 1; ++idx)
    //     std::cout << idx << " " << sub_tab[idx] << std::endl;

    return sub_tab;
}

void substitution_encrypt(std::string &str, std::string key)
{
    make_lower_case(str);

    std::unordered_map<char, char> sub_tab = create_sub_tab(key);

    for (char &character : str)
        // spaces don't get substituted
        if (character >= 'a' && character <= 'z')
            character = sub_tab[character];
}

void substitution_decrypt(std::string &str, std::string key)
{
    make_lower_case(str);

    std::unordered_map<char, char> sub_tab = create_sub_tab(key);
    std::unordered_map<char, char> swapped_sub_tab;
    // swap all pairs
    for (auto &pair : sub_tab)
        swapped_sub_tab.insert({pair.second, pair.first});

    for (char &character : str)
        // spaces don't get substituted
        if (character >= 'a' && character <= 'z')
            character = swapped_sub_tab[character];
}

// int main()
// {
//     std::string text = "DIE SCHWALBE HAT UNTER FOLTER ALLES VERRATEN STOP SOFORTIGER ABBRUCH VON OPERATION KLEOPATRA";
//     substitution_encrypt(text, "Nofretete");
//     std::cout << text << std::endl;
//     substitution_decrypt(text, "Nofretete");
//     std::cout << text << std::endl;
//
//     text = "GDY HENSLDYY GDY LXDB HEUEX QEEXZE, CEXFEEXZE NFXE QEUEXHEXZE";
//     substitution_decrypt(text, "dumm gelaufen");
//     std::cout << text << std::endl;
//
//     text = "ZQ EQR SJQ BZHSJBZJ, YZJ SQ-MPDZQEYZJRZJ YSPLC ZEJZJ SJQZPZP GHKJZ XS ZPQZRXZJI";
//     substitution_decrypt(text, "dolly");
//     std::cout << text << std::endl;
//
//     text = "ACHTUNGWIRWERDENABGEHOERT";
//     substitution_encrypt(text, "UFLPWDRASJMCONQYBVTEXHZKGI");
//     std::cout << text << std::endl;
//
//     text = "AUFWHWVTEUNPWN";
//     substitution_decrypt(text, "UFLPWDRASJMCONQYBVTEXHZKGI");
//     std::cout << text << std::endl;
//
//     text = "ZSV ZWVPWN XNT SN PWN NUWLATEWN OQNUEWN HWVTEUWVME XO PWN UXDFUX PWV QVRUNSTUESQN SN FVUNPWNFXVR MXWOOWVN. FWHQV ZSV RWOWSNTUO CQTTLACURWN, ZWVPWN ZSV UXD XNTWVWO EVWDDWN SN FWVCSN WSN NWXWT UCYAUFWE IXV HWVTLACXWTTWCXNR HWVWSNFUVWN";
//     substitution_decrypt(text, "UFLPWDRASJMCONQYBVTEXHZKGI");
//     std::cout << text << std::endl;
//
//     return 0;
// }

// output:
// rwe ifvmnzoe vnj kbjeh tczjeh nzzei lehhnjeb ijcd ictchjwueh noohkfv lcb cdehnjwcb yzecdnjhn
// die schwalbe hat unter folter alles verraten stop sofortiger abbruch von operation kleopatra
// das weinfass das frau weber leerte, verheerte ihre leberwerte
// es ist uns gelungen, den us-praesidenten durch einen unserer klone zu ersetzenm
// ulaexnrzsvzwvpwnufrwaqwve
// habeverstanden
// wir werden uns in den naechsten monaten verstaerkt um den aufbau der organisation in brandenburg kuemmern. bevor wir gemeinsam losschlagen, werden wir auf unserem treffen in berlin ein neues alphabet zur verschluesselung vereinbaren
