#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

full_path=$(realpath $0)
dir_path=$(dirname $full_path)

exec_path="$dir_path/../../bin/Dist-linux-x86_64/encryption/encryption"

# loop through file, encrypt each line
idx=0
while IFS= read -r line; do
    # skip empty lines
    if [[ "$line" != "" && "$line" != "#"* ]]; then
        printf "encrypting $idx\n"
        # encrypt
        command="$exec_path \"$line\" -a xor -k 0 -b 2 -0"
        IFS=$' \n\t'
        # encrypt, put into string, echo and pipe over, use tail to get last line only, reverse output, cut at tabs, take first element, reverse again
        output=$(/mnt/d/Files/cpp/cryptography/encryption/tests/../../bin/Dist-linux-x86_64/encryption/encryption "$line" -a xor -k 43 -b 16 -0)
        encryptions[idx]=$(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        idx=$(($idx+1))

        output=$(/mnt/d/Files/cpp/cryptography/encryption/tests/../../bin/Dist-linux-x86_64/encryption/encryption "$line" -a fence)
        encryptions[idx]=$(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        idx=$(($idx+1))

        output=$(/mnt/d/Files/cpp/cryptography/encryption/tests/../../bin/Dist-linux-x86_64/encryption/encryption "$line" -a plow -k 5)
        encryptions[idx]=$(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        idx=$(($idx+1))

        output=$(/mnt/d/Files/cpp/cryptography/encryption/tests/../../bin/Dist-linux-x86_64/encryption/encryption "$line" -a transpose -k 8)
        encryptions[idx]=$(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        idx=$(($idx+1))
        IFS=
    fi
done < "$dir_path/correct_sentences.txt"
# return IFS to be sane again
IFS=$'\n\t'

printf '%s\n' "${encryptions[@]}" > "$dir_path/ciphers.txt"

# 01010011011010000110010100100000011000010110010001110110011010010111001101100101011001000010000001101000011010010110110100100000011101000110111100100000011000110110111101101101011001010010000001100010011000010110001101101011001000000110000101110100001000000110111101101110011000110110010100101110
