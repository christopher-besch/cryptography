#include "Decrypted.h"

const int Decrypted::get_score()
{
    if (!m_score_calculated)
        calculate_score();
    return m_score;
}
