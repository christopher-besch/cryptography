#include "TransformDecrypt.h"

#include "Transformation.h"

/*
    a set of transformation, taking size of 2d array and vector-> moving vector to new location
*/
ElementParams plow_transform(ElementParams orig_array_params)
{
    // transpose
    int transposed_x = orig_array_params.y;
    int transposed_y = orig_array_params.x;
    int transposed_column_count = orig_array_params.row_count;
    int transposed_row_count = orig_array_params.column_count;

    // flip all rows
    int flipped_x = transposed_column_count - 1 - transposed_x;

    // flip every second column, last column gets flipped
    int flipped_y = transposed_y;
    if ((transposed_column_count - flipped_x) % 2)
        flipped_y = transposed_row_count - 1 - transposed_y;

    return {flipped_x, flipped_y, transposed_row_count, transposed_column_count};
}

ElementParams transpose_transform(ElementParams orig_array_params)
{
    // transpose
    int transposed_x = orig_array_params.y;
    int transposed_y = orig_array_params.x;
    int transposed_column_count = orig_array_params.row_count;
    int transposed_row_count = orig_array_params.column_count;

    return {transposed_x, transposed_y, transposed_row_count, transposed_column_count};
}

// go thorugh all keys for a specific transformation callback
void TransformDecryptor::test_decryptions(transformation_func transformation, TransformDecrypted &template_decrypt, bool row_count_known)
{
    for (int test_key = 1; test_key < m_cipher.size() + 1; test_key++)
        if (is_to_test_key(test_key))
        {
            template_decrypt.key = test_key;
            // create decryption
            if (row_count_known)
                template_decrypt.decrypted_str = transform_str(transformation, m_cipher, test_key, -1);
            else
                template_decrypt.decrypted_str = transform_str(transformation, m_cipher, -1, test_key);
            template_decrypt.score = m_dictionary.get_score(template_decrypt.decrypted_str);
            // when not filled yet
            if (!m_amount || m_decryptions.size() < m_amount)
                m_decryptions.push_back(template_decrypt);
            else
                // when space is rare, overwrite if good enough
                for (std::vector<TransformDecrypted>::iterator ptr = m_decryptions.begin(); ptr < m_decryptions.end(); ptr++)
                    if (*ptr < template_decrypt)
                    {
                        *ptr = template_decrypt;
                        break;
                    }
        }
}

void TransformDecryptor::create_decryptions(int amount)
{
    // more and more settings get stored in copies of this object
    TransformDecrypted template_decrypt;
    template_decrypt.transformation_type = "plow";
    test_decryptions(plow_transform, template_decrypt, true);
    template_decrypt.transformation_type = "transpose";
    test_decryptions(transpose_transform, template_decrypt, false);
}
