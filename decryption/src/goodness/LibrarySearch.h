#pragma once

#include "Trie.h"

// used to score different decryptions
class LibrarySearch
{
private:
    Trie m_dictionary;
    bool m_empty;

private:
    int get_word_score(std::string word) const;

public:
    LibrarySearch()
        : m_empty(true) {}
    LibrarySearch(const char *file_path)
        : m_empty(true)
    {
        load_file(file_path);
    }

    void load_file(std::string file_path);

    int get_score(std::string text) const;

    bool empty() const { return m_empty; }
};
