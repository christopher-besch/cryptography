#pragma once

#include <string>

// representing single vector representing single element in string + dimensions of virtual array
struct ElementParams
{
    int x;
    int y;

    int row_count;
    int column_count;
};

// callback used to transform a single vector representing an element in a string
typedef ElementParams (*transformation_func)(ElementParams);

// transform a single string using provided callback and either width or height of virtual array
std::string transform_str(transformation_func transformation, const std::string &str, int orig_row_count, int orig_column_count, char filler = '_');
