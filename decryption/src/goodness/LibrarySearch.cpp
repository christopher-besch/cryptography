#include "LibrarySearch.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "Utils.h"

float LibrarySearch::get_word_score(std::string word) const
{
    float score = 0.0f;
    // preprocess
    for (int idx = word.size() - 1; idx >= 0; idx--)
    {
        // make lower case
        if (word[idx] >= 'A' && word[idx] <= 'Z')
            word[idx] += 'a' - 'A';
        // remove and punish unsupported characters
        else if (word[idx] < 'a' || word[idx] > 'z')
        {
            // unsupported characters at the end are allowed (i.e. commas or full stops)
            if (idx != word.size() - 1)
                score -= 2.0f;
            word.erase(idx, 1);
        }
    }
    if (word.empty())
        return 0.0f;

    // scan through whole string searching for words
    // when word found skip to after last character of found word
    for (int start_idx = 0; start_idx < word.size();)
    {
        // add 1 for each matched character
        int matching_chars = m_dictionary.count_matching_chars(word.substr(start_idx, word.size() - start_idx));
        score += matching_chars;
        // all matching chars get skipped
        if (matching_chars)
            start_idx += matching_chars;
        // or one not matching char gets skipped
        else
            ++start_idx;
    }
    return score / word.size();
}

void LibrarySearch::load_file(const std::string &file_path)
{
    std::fstream file(file_path, std::ios::in);
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

float LibrarySearch::get_score(const std::string &text) const
{
    float score = 0;
    // the first unprintable character gets punished the most
    bool found_unprintable = false;
    for (auto ptr = text.begin(); ptr < text.end(); ptr++)
        if (*ptr < ' ' && *ptr != '\n' && *ptr != '\r')
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
