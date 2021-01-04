#include "LibrarySearch.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "Utils.h"

// evaluates single word
int LibrarySearch::get_word_score(std::string word) const
{
    int score = 0;
    // remove unsupported characters
    for (int idx = word.size() - 1; idx >= 0; idx--)
        if (!(word[idx] >= 'a' && word[idx] <= 'z' || word[idx] >= 'A' && word[idx] <= 'Z'))
        {
            // unsupported characters at the end are allowed
            // todo: only not punish commas and such
            if (idx != word.size() - 1)
                score -= 2;
            word.erase(idx, 1);
        }

    // make lower-case
    for (int idx = 0; idx < word.size(); idx++)
    {
        if (word[idx] >= 'A' && word[idx] <= 'Z')
        {
            word[idx] += 'a' - 'A';
            // the first letter can be capital without any punishment
            if (idx != 0)
                score--;
        }
    }

    score += m_dictionary.search(word) * word.size() * 2;

    // capital letters are half as good as lower case ones
    return score;
}

// load dictionary into Trie
void LibrarySearch::load_file(std::string file_path)
{
    std::ifstream file(file_path);
    if (!file)
        raise_error("Can't open dictionary " << file_path);

    for (std::string buffer; std::getline(file, buffer);)
    {
        // remove unsupported characters
        // todo: weird \r
        for (int idx = buffer.size() - 1; idx >= 0; idx--)
        {
#ifdef DEBUG
            if (buffer[idx] >= 'A' && buffer[idx] <= 'Z')
                raise_error("'" << buffer << "' in file " << file_path << " is invalid!");
#endif
            if (buffer[idx] < 'a' || buffer[idx] > 'z')
                buffer.erase(idx, 1);
        }
        m_dictionary.insert(buffer);
    }
    file.close();
}

// evaluate whole decrypted text
int LibrarySearch::get_score(std::string text) const
{
    int score = 0;
    // the first unprintable character gets punished the most
    bool found_unprintable = false;
    for (std::string::iterator ptr = text.begin(); ptr < text.end(); ptr++)
        if (*ptr < ' ' || *ptr > '~')
        {
            found_unprintable = true;
            score -= 10;
        }
    score -= found_unprintable * 1000;

    // evalute each word on its own
    std::stringstream ss_text(text);
    for (std::string word; std::getline(ss_text, word, ' ');)
        score += get_word_score(word);
    return score;
}
