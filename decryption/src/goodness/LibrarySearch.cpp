#include "LibrarySearch.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>

#include "Utils.h"

// todo: not working!!!
// todo: no word delimiters
// evaluates single word
float LibrarySearch::get_word_score(std::string word) const
{
    float score = 0.0f;
    // remove unsupported characters
    for (int idx = word.size() - 1; idx >= 0; idx--)
        if (!(word[idx] >= 'a' && word[idx] <= 'z' || word[idx] >= 'A' && word[idx] <= 'Z'))
        {
            // unsupported characters at the end are allowed
            // todo: only not punish commas and such
            if (idx != word.size() - 1)
                score -= 2.0f;
            word.erase(idx, 1);
        }

    if (word.empty())
        return 0.0f;

    make_lower_case(word);

    // empty word is no use -> noone cares about last element
    // todo: for (int start_idx = 0; start_idx < uncut_word.size() - 1; ++start_idx)
    for (int start_idx = 0; start_idx < word.size();)
    {
        // todo: this is broken!!!
        int matching_chars = m_dictionary.count_matching_chars(word.substr(start_idx, word.size() - start_idx));
        score += std::pow(2, matching_chars);
        // all matching chars get skipped
        if (matching_chars)
            start_idx += matching_chars;
        // or one not matching char gets skipped
        else
            ++start_idx;
    }
    return score / std::pow(2, word.size());
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
float LibrarySearch::get_score(const std::string &text) const
{
    float score = 0;
    // the first unprintable character gets punished the most
    bool found_unprintable = false;
    for (auto ptr = text.begin(); ptr < text.end(); ptr++)
        // todo: odd characters
        if (*ptr < ' ') //|| *ptr == '\127' || *ptr == '\129' || *ptr == '\141' || *ptr == '\143' || *ptr == '\144' || *ptr == '\157' || *ptr == '\160')
        {
            found_unprintable = true;
            score -= 10.0f;
        }
    score -= found_unprintable * 1000.0f;

    // evalute each word on its own
    std::stringstream ss_text(text);
    for (std::string word; std::getline(ss_text, word, ' ');)
        score += get_word_score(word);
    return score;
}
