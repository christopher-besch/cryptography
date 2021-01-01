#include "Trie.h"

void Trie::insert(std::string new_key)
{
#ifdef DEBUG
    for (char character : new_key)
        if (character < 'a' || character > 'z')
            raise_error(new_key << "is an invalid key for Trie!");
#endif

    // start at root node
    TrieNode *current_node = m_root;
    for (int idx = 0; idx < new_key.size(); idx++)
    {
        int char_idx = new_key[idx] - 'a';
        // when not found
        if (!current_node->children[char_idx])
            current_node->children[char_idx] = new TrieNode;
        current_node = current_node->children[char_idx];
    }
    // last node is leaf
    current_node->end_of_word = true;
}

bool Trie::search(std::string key)
{
#ifdef DEBUG
    for (char character : key)
        if (character < 'a' || character > 'z')
            raise_error(key << "is an invalid key for Trie!");
#endif
    // start at root node
    TrieNode *current_node = m_root;
    for (int idx = 0; idx < key.size(); idx++)
    {
        int char_idx = key[idx] - 'a';
        // when not found
        if (!current_node->children[char_idx])
            return false;
        current_node = current_node->children[char_idx];
    }
    // only when existent and leaf
    return current_node && current_node->end_of_word;
}