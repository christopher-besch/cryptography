#include "Decrypted.h"

void Decrypted::calculate_score()
{
    // score used to compare different decryptions
    m_score = 0;
    for (char character : m_decrypted_text)
    {
        // for each printable character
        if (character >= ' ' && character <= '~')
            m_score += 1;
        else
            m_score -= 1;

        // extra points for "good" characters
        if (character >= 'A' && character <= 'Z')
            m_score += 2;
        else if (character >= 'a' && character <= 'z')
            m_score += 2;
        else if (character == ' ')
            m_score += 1;
    }
    m_score *= m_char_length ? m_char_length : 8;
}

Decrypted::Decrypted(std::string decrypted_text, char delimiter, int char_length)
    : m_decrypted_text(decrypted_text), m_delimiter(delimiter), m_char_length(char_length)
{
    calculate_score();
}
