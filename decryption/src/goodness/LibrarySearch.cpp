#include "LibrarySearch.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "Utils.h"

// todo: not working!!!
// todo: no word delimiters
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

    int matching_chars = m_dictionary.count_matching_chars(word);
    // punished if whole word doesn't match
    if (matching_chars != word.size())
        matching_chars /= 2;

    score += m_dictionary.count_matching_chars(word) * 2;
    // capital letters are half as good as lower case ones
    return score;
}

int LibrarySearch::get_uncut_word_score(const std::string &uncut_word) const
{
    int score = 0;
    score += get_word_score(uncut_word.substr(0, uncut_word.size()));

    // empty word is no use -> noone cares about last element
    // todo: for (int start_idx = 0; start_idx < uncut_word.size() - 1; ++start_idx)
    for (int start_idx = 1; start_idx < uncut_word.size(); ++start_idx)
    {
        score += get_word_score(uncut_word.substr(start_idx, uncut_word.size() - start_idx)) / 2;
    }
    return score;
}

// load dictionary into Trie
void LibrarySearch::load_file(const std::string &file_path)
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
int LibrarySearch::get_score(const std::string &text) const
{
    int score = 0;
    // the first unprintable character gets punished the most
    bool found_unprintable = false;
    for (auto ptr = text.begin(); ptr < text.end(); ptr++)
        if (*ptr < ' ' || *ptr > '~')
        {
            found_unprintable = true;
            score -= 10;
        }
    score -= found_unprintable * 1000;

    // evalute each word on its own
    std::stringstream ss_text(text);
    for (std::string word; std::getline(ss_text, word, ' ');)
        score += get_uncut_word_score(word);
    return score;
}
