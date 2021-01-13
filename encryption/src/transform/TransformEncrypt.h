#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

#include <Utils.h>
#include <Transformation.h>

#include "Encrypted.h"

class TransposeEncrypted : public Encrypted
{
private:
    int m_key;

public:
    TransposeEncrypted(std::string encrypted_string, int key)
        : Encrypted(encrypted_string), m_key(key) {}

    int get_key() const { return m_key; }
};

// equivalent to: fill row by row, read column by column like a snake
inline ElementParams plow_transform(ElementParams orig_array_params)
{
    // flip every second column, last column gets flipped
    int flipped_y = orig_array_params.y;
    if ((orig_array_params.column_count - orig_array_params.x) % 2)
        flipped_y = orig_array_params.row_count - 1 - orig_array_params.y;

    // flip all rows
    int flipped_x = orig_array_params.column_count - 1 - orig_array_params.x;

    // transpose
    int transposed_x = flipped_y;
    int transposed_y = flipped_x;
    int transposed_row_count = orig_array_params.column_count;
    int transposed_column_count = orig_array_params.row_count;

    return {transposed_x, transposed_y, transposed_row_count, transposed_column_count};
}

// equivalent to: fill row by row, read column by column
inline ElementParams transpose_transform(ElementParams orig_array_params)
{
    // transpose
    int transposed_x = orig_array_params.y;
    int transposed_y = orig_array_params.x;
    int transposed_row_count = orig_array_params.column_count;
    int transposed_column_count = orig_array_params.row_count;

    return {transposed_x, transposed_y, transposed_row_count, transposed_column_count};
}

// perform transformation for whole string using supplied transformation callback
inline TransposeEncrypted transform_encrypt(const std::string &str, transformation_func transformation, int key, char filler = '_')
{
    std::string result = transform_str(transformation, str, -1, key, filler);
    return {result, key};
}
