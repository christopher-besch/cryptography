#pragma once

#include <string>

#include "Utils.h"

struct ElementParams
{
    int x;
    int y;

    int row_count;
    int column_count;
};

typedef ElementParams (*translation_func)(ElementParams);

// fill virtual array row by row
// go thorugh all elements and give coordinates of each element and dimensions or virtual array to transformation function
// move every element according to transformation function
// read new array row by row
inline std::string transpose_str(translation_func transformation, const std::string &str, int original_row_count, int original_column_count, char filler = '_')
{
    // when only row count known
    if (original_column_count < 1 || original_row_count >= 1)
    {
        int original_column_count = str.size() / original_row_count;
        if (str.size() % original_row_count)
            ++original_column_count;
    }
    // when only column count known
    else if (original_column_count >= 1 || original_row_count < 1)
    {
        int original_row_count = str.size() / original_column_count;
        if (str.size() % original_column_count)
            ++original_row_count;
    }
    // when both row and column count known or neither
    else
        raise_error("Column count " << original_column_count << " and row count " << original_row_count << " are invalid!");

    std::string result(original_row_count * original_column_count, filler);
    // go through all elements
    ElementParams array_params{-1, -1, original_row_count, original_column_count};
    for (int idx = 0; idx < str.size(); ++idx)
    {
        // fill virtual array row by row (original rows)
        array_params.x = idx % original_column_count;
        array_params.y = idx / original_column_count;

        // do transformation
        ElementParams transformed_array_params = transformation(array_params);

        // write character row by row (new rows)
        int new_idx = transformed_array_params.y * transformed_array_params.column_count + transformed_array_params.x;
        result[new_idx] = str[idx];
    }
    return result;
}