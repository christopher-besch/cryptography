#include "XORDecrypted.h"

#include <math.h>

void XORDecrypted::calculate_score()
{
    // todo: maybe Trie good?
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
            m_score += 3;
    }
    m_score += std::abs(m_score * m_char_length);

    if (m_key == 0)
        m_score += std::abs(m_score * 0.5);
    if (m_base == 2 || m_base == 8 || m_base == 10 || m_base == 16)
        m_score += std::abs(m_score * 0.5);

    m_score_calculated = true;
}
