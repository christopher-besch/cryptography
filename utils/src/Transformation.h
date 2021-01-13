#pragma once

#include <string>

struct ElementParams
{
    int x;
    int y;

    int row_count;
    int column_count;
};

typedef ElementParams (*transformation_func)(ElementParams);

std::string transform_str(transformation_func transformation, const std::string &str, int orig_row_count, int orig_column_count, char filler = '_');
