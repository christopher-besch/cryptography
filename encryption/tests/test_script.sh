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
        printf "encrypting line $idx\n"
        # encrypt
        command="$exec_path \"$line\" -a xor -k 0 -b 2 -0"
        IFS=$' \n\t'
        # encrypt, put into string, echo and pipe over, use tail to get last line only, reverse output, cut at tabs, take first element, reverse
        # printf "%s\n" "$($command)"
        # output=$($command)
        output=$(/mnt/d/Files/cpp/cryptography/encryption/tests/../../bin/Dist-linux-x86_64/encryption/encryption "She advised him to come back at once." -a xor -k 0 -b 2 -0)
        eval $command
        echo $command
        echo $output
        encryptions[idx]=$(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        echo $(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        IFS=
        # update line index
        idx=$(($idx+1))
    fi
done < "$dir_path/correct_sentences.txt"
# return IFS to be sane again
IFS=$'\n\t'

printf '%s\n\n' "${encryptions[@]}"

# 01010011011010000110010100100000011000010110010001110110011010010111001101100101011001000010000001101000011010010110110100100000011101000110111100100000011000110110111101101101011001010010000001100010011000010110001101101011001000000110000101110100001000000110111101101110011000110110010100101110
