#include "Decrypted.h"

void Decrypted::calculate_score()
{
    m_score = 0;
    for (char character : decrypted_text)
    {
        // one point for each printable character
        if (character >= ' ' && character <= '~')
            m_score += 1;
        else
            m_score -= 1;

        // extra points for "good" character
        if (character >= 'A' && character <= 'Z')
            m_score += 2;
        else if (character >= 'a' && character <= 'z')
            m_score += 2;
        else if (character == ' ')
            m_score += 1;
    }
}

Decrypted::Decrypted(std::string decrypted_text, char delimiter)
    : decrypted_text(decrypted_text), m_delimiter(delimiter)
{
    calculate_score();
}
