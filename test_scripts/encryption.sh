#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

full_path=$(realpath $0)
dir_path=$(dirname $full_path)

# loop through file, encrypt each line
idx=0
while IFS= read -r line; do
    # skip empty lines and comments
    if [[ "$line" != "" && "$line" != "#"* ]]; then
        # encrypt
        IFS=$' \n\t'
        # encrypt, put into string, echo and pipe over, use tail to get last line only, reverse output, cut at tabs, take first element, reverse again
        output=$("${dir_path}/../bin/Dist-linux-x86_64/encryption/encryption" "$line" -a xor -k 43 -b 16 -0)
        encryptions[idx]=$(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        idx=$(($idx+1))
        printf "encrypted $idx\n"

        output=$("${dir_path}/../bin/Dist-linux-x86_64/encryption/encryption" "$line" -a fence)
        encryptions[idx]=$(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        idx=$(($idx+1))
        printf "encrypted $idx\n"

        output=$("${dir_path}/../bin/Dist-linux-x86_64/encryption/encryption" "$line" -a plow -k 5)
        encryptions[idx]=$(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        idx=$(($idx+1))
        printf "encrypted $idx\n"

        output=$("${dir_path}/../bin/Dist-linux-x86_64/encryption/encryption" "$line" -a transpose -k 8)
        encryptions[idx]=$(echo "$output" | tail -1 | rev | cut -d$'\t' -f1 | rev)
        idx=$(($idx+1))
        printf "encrypted $idx\n"
        IFS=
    fi
done < "$dir_path/input1.txt"
# return IFS to be sane again
IFS=$'\n\t'

printf '%s\n' "${encryptions[@]}" > "$dir_path/ciphers1.txt"
