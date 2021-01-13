#pragma once

#include "Trie.h"

// used to score different decryptions
class LibrarySearch
{
private:
    Trie m_dictionary;

private:
    int get_word_score(std::string word) const;
    int get_uncut_word_score(const std::string &uncut_word) const;

public:
    LibrarySearch() {}
    LibrarySearch(const std::string &file_path)
    {
        load_file(file_path);
    }
    // copies are not allowed
    LibrarySearch(const LibrarySearch &) = delete;
    LibrarySearch &operator=(const LibrarySearch) = delete;

    void load_file(const std::string &file_path);

    int get_score(const std::string &text) const;
};

// todo: m_empty bad
