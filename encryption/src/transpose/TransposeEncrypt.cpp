#include "TransposeEncrypt.h"

#include <string>

TransposeEncrypted plow_encrypt(const std::string &str, int original_column_count, char filler)
{
    if (original_column_count < 1)
        raise_error("Unsupported column amount: " << original_column_count);
    int original_row_count = str.size() / original_column_count;
    // add column when at least one character in that column
    if (str.size() % original_column_count)
        ++original_row_count;

    std::string result(original_row_count * original_column_count, filler);

    for (int idx = 0; idx < str.size(); ++idx)
    {
        // fill virtual array row by row (original rows)
        int original_x = idx % original_column_count;
        int original_y = idx / original_column_count;

        // flip every second column, last column gets flipped
        int flipped_y = original_y;
        if ((original_column_count - original_x) % 2)
            flipped_y = original_row_count - 1 - original_y;

        // flip all rows
        int flipped_x = original_column_count - 1 - original_x;

        // transpose
        int transposed_x = flipped_y;
        int transposed_y = flipped_x;
        int transposed_column_count = original_row_count;
        int transposed_row_count = original_column_count;

        // write character row by row (new rows)
        result[transposed_y * transposed_column_count + transposed_x] = str[idx];
    }
    return {result, original_column_count};
}

TransposeEncrypted transpose_encrypt(const std::string &str, int original_column_count, char filler)
{
    if (original_column_count < 1)
        raise_error("Unsupported column amount: " << original_column_count);
    // add column when at least one character in that column
    int original_row_count = str.size() / original_column_count;
    if (str.size() % original_column_count)
        ++original_row_count;

    std::string result(original_row_count * original_column_count, filler);

    for (int idx = 0; idx < str.size(); ++idx)
    {
        // fill virtual array row by row (original rows)
        int original_x = idx % original_column_count;
        int original_y = idx / original_column_count;

        // transpose
        int transposed_x = original_y;
        int transposed_y = original_x;
        int transposed_column_count = original_row_count;
        int transposed_row_count = original_column_count;

        // write character row by row (new rows)
        result[transposed_y * transposed_column_count + transposed_x] = str[idx];
    }
    return {result, original_row_count};
}
