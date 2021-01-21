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
inline ElementParams plow_encrypt_transform(ElementParams orig_array_params)
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
inline ElementParams transpose_encrypt_transform(ElementParams orig_array_params)
{
    // transpose
    int transposed_x = orig_array_params.y;
    int transposed_y = orig_array_params.x;
    int transposed_row_count = orig_array_params.column_count;
    int transposed_column_count = orig_array_params.row_count;

    return {transposed_x, transposed_y, transposed_row_count, transposed_column_count};
}

ElementParams plow_decrypt_transform(ElementParams orig_array_params)
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

ElementParams transpose_decrypt_transform(ElementParams orig_array_params)
{
    // transpose
    int transposed_x = orig_array_params.y;
    int transposed_y = orig_array_params.x;
    int transposed_column_count = orig_array_params.row_count;
    int transposed_row_count = orig_array_params.column_count;

    return {transposed_x, transposed_y, transposed_row_count, transposed_column_count};
}

ElementParams block_transform1(ElementParams orig_array_params)
{
    int new_x = -1;
    switch (orig_array_params.x)
    {
    case 0:
        new_x = 2;
        break;
    case 1:
        new_x = 5;
        break;
    case 2:
        new_x = 4;
        break;
    case 3:
        new_x = 3;
        break;
    case 4:
        new_x = 0;
        break;
    case 5:
        new_x = 1;
        break;
    }

    return {new_x, orig_array_params.y, orig_array_params.row_count, orig_array_params.column_count};
}

ElementParams block_transform2(ElementParams orig_array_params)
{
    int new_x = -1;
    switch (orig_array_params.x)
    {
    case 0:
        new_x = 1;
        break;
    case 1:
        new_x = 3;
        break;
    case 2:
        new_x = 2;
        break;
    case 3:
        new_x = 0;
        break;
    case 4:
        new_x = 5;
        break;
    case 5:
        new_x = 7;
        break;
    case 6:
        new_x = 4;
        break;
    case 7:
        new_x = 6;
        break;
    }

    return {new_x, orig_array_params.y, orig_array_params.row_count, orig_array_params.column_count};
}

ElementParams block_transform3(ElementParams orig_array_params)
{
    int new_x = -1;
    switch (orig_array_params.x)
    {
    case 0:
        new_x = 3;
        break;
    case 1:
        new_x = 0;
        break;
    case 2:
        new_x = 2;
        break;
    case 3:
        new_x = 1;
        break;
    case 4:
        new_x = 6;
        break;
    case 5:
        new_x = 4;
        break;
    case 6:
        new_x = 7;
        break;
    case 7:
        new_x = 5;
        break;
    }

    return {new_x, orig_array_params.y, orig_array_params.row_count, orig_array_params.column_count};
}

// int main()
// {
//     // std::cout << "fence:" << std::endl;
//     // std::cout << fence_encrypt("liebertugendhaftalsjugendhaft") << std::endl;
//     // std::cout << fence_encrypt("HABEAMSONNTAGKEINEZEITMUSSLERNENSORRY") << std::endl;
//     // std::cout << fence_decrypt("LEEMTEOAEENLMTITROLNIBRIVRNKGLASIDEEBHE") << std::endl;
//     // std::cout << fence_decrypt("AMTWCFLEDERTNWITNEASMITOHALNIESEZESUDNU") << std::endl;
//     //
//     // std::cout << "\nplow:" << std::endl;
//     // std::cout << transform_str(plow_encrypt_transform, "derschatzliegtunterdenpalmen", -1, 6) << std::endl;
//     //
//     // std::cout << transform_str(plow_decrypt_transform, "_aeehcitp_nnnlsrzueemdttedagrl", 6, -1) << std::endl;
//     // std::cout << transform_str(plow_decrypt_transform, "XGCNEITMISRSEHIEHTCIDAHE", 6, -1) << std::endl;
//     //
//     // std::cout << "\ntranspose:" << std::endl;
//     // // todo: broken
//     // std::cout << transform_str(transpose_encrypt_transform, "0123456789abcdefghijklmnop", -1, 6) << std::endl;
//     // std::cout << transform_str(transpose_encrypt_transform, transform_str(transpose_encrypt_transform, "ALLESGUTEZUMGEBURTSTAGANNEGRET!", -1, 6), 6, -1) << std::endl;
//     //
//     // std::cout << transform_str(transpose_encrypt_transform, "ALLESGUTEZUMGEBURTSTAGANNEGRET", -1, 6) << std::endl;
//     //
//     // std::cout << transform_str(transpose_decrypt_transform, "AUGSNLTETELEBAGEZUGRSURAEGMTNT", -1, 6) << std::endl;
//     // std::cout << transform_str(transpose_decrypt_transform, "KUITKSOHTZEPMETUTIMUAMBESTGBALTEMALEDMISLN", -1, 6) << std::endl;
//     //
//     // std::cout << "\nblock:" << std::endl;
//     // std::cout << transform_str(block_transform1, "dasgeschenkliegtunterdemstuhl", -1, 6) << std::endl;
//     // std::cout << transform_str(block_transform2, "wirtreffenunsumdreiuhr", -1, 8) << std::endl;
//     // std::cout << transform_str(block_transform3, "TDREFRFEKPNUSTTIAAHMNFDE", -1, 8) << std::endl;
// }

int main(int argc, char *argv[])
{
    for (int idx = 0; idx < argc; ++idx)
        std::cout << argv[idx] << std::endl;
}
