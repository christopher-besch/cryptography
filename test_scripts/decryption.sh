#!/bin/bash
set -euo pipefail
IFS=$'\n\t'

full_path=$(realpath $0)
dir_path=$(dirname $full_path)

printf "" > ${dir_path}/decrypted_ciphers.txt

${dir_path}/../bin/Dist-linux-x86_64/decryption/decryption -f ${dir_path}/ciphers1.txt ${dir_path}/output1.txt
${dir_path}/../bin/Dist-linux-x86_64/decryption/decryption -f ${dir_path}/ciphers2.txt ${dir_path}/output2.txt
