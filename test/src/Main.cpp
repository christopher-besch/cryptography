#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Utils.h"

std::string fence_encrypt(std::string str)
{
    std::string first;
    std::string second;

    // add first character to first, second to second, third to first and so on
    for (int idx = 0; idx < str.size(); ++idx)
    {
        if (idx % 2)
            second.push_back(str[idx]);
        else
            first.push_back(str[idx]);
    }
    // return second glued to first
    return first.append(second);
}

std::string fence_decrypt(std::string str)
{
    // the first part has one more character than the second one if odd amount of characters
    std::string first;
    std::string second;
    if (str.size() % 2)
    {
        first = str.substr(0, str.size() / 2 + 1);
        second = str.substr(str.size() / 2 + 1, str.size() / 2);
    }
    else
    {
        first = str.substr(0, str.size() / 2);
        second = str.substr(str.size() / 2, str.size() / 2);
    }

    // add character from first and then from second until done
    std::string result;
    for (int idx = 0; idx < first.size(); ++idx)
    {
        result.push_back(first[idx]);
        result.push_back(second[idx]);
    }
    return result;
}

// fill row by row, read column by column like a snake
std::string plow_encrypt(const std::string &str, int original_column_count, char filler = ' ')
{
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
    return result;
}

// fill column by column like a snake, read row by row
std::string plow_decrypt(const std::string &str, int original_row_count, char filler = ' ')
{
    int original_column_count = str.size() / original_row_count;
    // add column when at least one character in that column
    if (str.size() % original_row_count)
        ++original_column_count;

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

        // flip all rows
        int flipped_x = transposed_column_count - 1 - transposed_x;

        // flip every second column, last column gets flipped
        int flipped_y = transposed_y;
        if ((transposed_column_count - flipped_x) % 2)
            flipped_y = transposed_row_count - 1 - transposed_y;

        // write character row by row (new rows)
        result[flipped_y * transposed_column_count + flipped_x] = str[idx];
    }
    return result;
}

// std::string plow_decrypt(std::string str, int columns_count)
// {
//     // always rectangle
//     if (str.size() % columns_count)
//         return "";
//     // raise_error("The cipher '" << str << "' doesn't fit into a rectangle with the sidelength " << columns_count << "!");
//     int rows_count = str.size() / columns_count;
//
//     // create and fill column by column
//     std::vector<std::string> columns(columns_count);
//     for (int idx = 0; idx < str.size(); idx += rows_count)
//     {
//         // fill rightmost column first
//         int column_idx = columns_count - 1 - idx / rows_count;
//         columns[column_idx] = str.substr(idx, rows_count);
//         // reverse every second column
//         if (column_idx % 2)
//             std::reverse(columns[column_idx].begin(), columns[column_idx].end());
//     }
//
//     // read row by row
//     std::string result;
//     for (int row_idx = 0; row_idx < rows_count; ++row_idx)
//         for (std::string column : columns)
//             result.push_back(column[row_idx]);
//     return result;
// }

// fill 2d array row by row and read column by column
std::string transpose(const std::string &str, int row_count, int column_count, char filler = ' ')
{
    std::string result(row_count * column_count, filler);

    for (int idx = 0; idx < str.size(); ++idx)
    {
        // fill virtual array row by row
        int original_x = idx % row_count;
        int original_y = idx / row_count;

        // transpose
        int new_x = original_y;
        int new_y = original_x;
        // write character
        result[new_y * column_count + new_x] = str[idx];
    }
    return result;
}

// column_count known
std::string transpose_rows_known(const std::string &str, int key, char filler = ' ')
{
    // add column when at least one character in that column
    int column_count = str.size() / key;
    if (str.size() % key)
        ++column_count;
    return transpose(str, key, column_count, filler);
}

// row_cout known
std::string transpose_columns_known(const std::string &str, int key, char filler = ' ')
{
    // add row when at least one character in that row
    int row_count = str.size() / key;
    if (str.size() % key)
        ++row_count;
    return transpose(str, row_count, key, filler);
}

int main()
{
    std::cout << plow_encrypt("derschatzliegtunterderpalme", 6, 'x') << std::endl;
    // std::cout << plow_encrypt("0123456789abcdefgh", 6) << std::endl;
    std::cout << plow_decrypt("xaeehcitpxxrnlsrzueemdttedagrl", 6, 'x') << std::endl;
    std::cout << plow_decrypt("HIHANNKEGCECAOITKSACSNSFNTRIAD", 6, 'x') << std::endl;
    std::cout << plow_decrypt("XGCNEITMISRSEHIEHTCIDAHE", 6, 'x') << std::endl;
}
