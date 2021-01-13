#include "Transformation.h"

#include "Utils.h"

// fill virtual array row by row
// go thorugh all elements and give coordinates of each element and dimensions or virtual array to transformation callback
// move every element according to transformation function
// read new array row by row
std::string transform_str(transformation_func transformation, const std::string &str, int orig_row_count, int orig_column_count, char filler)
{
    // when only row count known
    if (orig_column_count < 1 || orig_row_count >= 1)
    {
        orig_column_count = str.size() / orig_row_count;
        if (str.size() % orig_row_count)
            ++orig_column_count;
    }
    // when only column count known
    else if (orig_column_count >= 1 || orig_row_count < 1)
    {
        orig_row_count = str.size() / orig_column_count;
        if (str.size() % orig_column_count)
            ++orig_row_count;
    }
    // when both row and column count known or neither
    else
        raise_error("Column count " << orig_column_count << " and row count " << orig_row_count << " are invalid!");

    std::string result(orig_row_count * orig_column_count, filler);
    // go through all elements
    ElementParams orig_array_params{-1, -1, orig_row_count, orig_column_count};
    for (int idx = 0; idx < str.size(); ++idx)
    {
        // fill virtual array row by row (original rows)
        orig_array_params.x = idx % orig_column_count;
        orig_array_params.y = idx / orig_column_count;

        // do transformation
        ElementParams transformed_array_params = transformation(orig_array_params);

        // write character row by row (new rows)
        int new_idx = transformed_array_params.y * transformed_array_params.column_count + transformed_array_params.x;
        result[new_idx] = str[idx];
    }
    return result;
}
