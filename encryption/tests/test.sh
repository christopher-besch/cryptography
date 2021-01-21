#!/bin/bash
set -euo pipefail
IFS=$' '

path="/mnt/d/Files/cpp/cryptography/encryption/tests/../../bin/Dist-linux-x86_64/test/test"

str="She advised him to come back at once."

command="\"${str}\"-a xor -k 0 -b 2 -0"
# echo $command
output=$($path "$command")
echo $output

# output=$(/mnt/d/Files/cpp/cryptography/encryption/tests/../../bin/Dist-linux-x86_64/encryption/encryption "She advised him to come back at once." -a xor -k 0 -b 2 -0)
# echo $output
