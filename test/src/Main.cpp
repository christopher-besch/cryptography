#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "Utils.h"
#include "Transformation.h"
#include "Trie.h"

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

// equivalent to: fill row by row, read column by column like a snake
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

// equivalent to: fill column by column like a snake, read row by row
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

// equivalent to: fill row by row, read column by column
std::string transpose_encrypt(const std::string &str, int original_row_count, char filler = ' ')
{
    // add column when at least one character in that column
    int original_column_count = str.size() / original_row_count;
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

        // write character row by row (new rows)
        result[transposed_y * transposed_column_count + transposed_x] = str[idx];
    }
    return result;
}

// equivalent to: fill column by column, read row by row
std::string transpose_decrypt(const std::string &str, int original_column_count, char filler = ' ')
{
    // add row when at least one character in that row
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
    return result;
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

int main()
{
    // std::cout << transform_str(transpose_transform, "05afkp16bgl 27chm 38din 49ejo ", -1, 6) << std::endl;

    Trie t;
    t.insert("marca");
    t.insert("marc");
    t.insert("ma");
    t.insert("arc");
    std::cout << t.count_matching_chars("marca") << std::endl;
    std::cout << t.count_matching_chars("arcafsdakjh") << std::endl;
}
