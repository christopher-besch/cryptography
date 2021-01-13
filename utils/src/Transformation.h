#pragma once

#include <string>

struct ElementParams
{
    int x;
    int y;

    int row_count;
    int column_count;
};

std::string transform_str(ElementParams (*transformation)(ElementParams), const std::string &str, int orig_row_count, int orig_column_count, char filler = '_');
