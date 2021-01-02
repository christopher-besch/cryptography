#pragma once

#include "Trie.h"

// used to score different decryptions
class LibrarySearch
{
private:
    Trie m_dictionary;

private:
    int get_word_score(std::string word) const;

public:
    LibrarySearch() = default;
    LibrarySearch(const char *file_path)
    {
        load_file(file_path);
    }

    void load_file(const char *file_path);

    int get_score(std::string text) const;
};
