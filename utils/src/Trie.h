#pragma once

#include <string>

#include "Utils.h"

const int alphabet_size = 26;

struct TrieNode
{
    bool end_of_word;
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

public:
    Trie()
        : m_root(new TrieNode) {}
    ~Trie()
    {
        delete m_root;
    }

    void insert(std::string new_key);
    bool search(std::string key);
};
