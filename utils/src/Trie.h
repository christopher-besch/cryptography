#pragma once

#include <string>

#include "Utils.h"

const int alphabet_size = 26;

struct TrieNode
{
    // when this is leaf node
    bool end_of_word;
    // either nullptr or pointing to next node
    TrieNode *children[alphabet_size];

    TrieNode()
        : end_of_word(false)
    {
        for (int idx = 0; idx < alphabet_size; idx++)
            children[idx] = nullptr;
    }

    ~TrieNode()
    {
        for (int idx = 0; idx < alphabet_size; idx++)
            delete children[idx];
    }
};

class Trie
{
private:
    TrieNode *m_root;

private:
    int char_to_idx(char character) const
    {
#ifdef DEBUG
        if (character < 'a' || character > 'z')
            raise_error(character << "is an invalid character for Trie!");
#endif
        return character - 'a';
    }

public:
    Trie()
        : m_root(new TrieNode) {}
    ~Trie()
    {
        delete m_root;
    }

    void insert(std::string new_key);
    bool search(std::string key) const;
};
