# 🔒Cryptography

This repository contains two console applications, `encryption` and `decryption`.
Both programs are designed to be executed from the command line.

## Encryption

`encryption` encrypts the input using one of the supported algorithms
To select the desired algorithm, the parameter `-a <algorithm name>` or `--algorithm <algorithm name>` shall be used.
Each algorithms uses certain settings/keys.
If one of setting is supplied as a console line parameter, only this value for this setting will be used.
If the setting is not supplied, all possible values for this setting will be used in succession, returning multiple different ciphers.

The input text can be supplied as the first parameter (otherwise the user has to input it later on).

### Algorithms

- `xor`:
  - `-k <key>` or `--key <key>`: each number (representing a single character using ASCII) get xored with this number
  - `-b <base>` or `--base <base>`: sets base, every encrypted number shall be represented in
  - `-d <delimiter>` or `--delim <delimiter>`: if not supplied, the encrypted numbers won't be separated, if the `<delimiter>` is omitted, a space will be used
  - `-o`: when set, each encrypted numbers will have the length (0s will be used as a padding)
- `plow`:
  `-k <key>`: sets the width of the array, the input gets read into
- `transpose`:
  - `-k <key>`: sets the width of the array, the input gets read into
- `fence`

### Example

These examples are from linux, windows users have to replace `./encryption` with `encryption.exe`.

```
./encryption "Hello World!" -a xor -b 16 -k 42 -0 -d
```

This command returns the cipher `62 4f 46 46 45 0a 7d 45 58 46 4e 0b` using the xor encryption algorithm with the key 42. The encrypted characters are represented in base-16 (hex), separated with spaces and padded with 0s.

## Decryption

This is a console application able to decrypt certain ciphers automatically.
The user gives the program as much information as she has and it tries its best to decrypt the entered cipher.
The program usually a brute force method by default, creating all decryptions using all possible settings for the decryption algorithms.
Each decrypted string gets a score based on its human comprehensibility.
To determine the score of a given string, a dictionary of English and German words is being used.
The user is able to add custom words into `user_dict.dic` in the `resource` folder.

Each supported algorithm works in a slightly different way.

- `-f <input_file_path> <output_file_path>` or `--file <input_file_path> <output_file_path>`: When this flag is used, the output will also be stored in a file at `<file_path>` or "output.txt" if omitted.
- `-a <amount>` or `--amount <amount>`: The program returns the five best decryptions it can find.
  This amount can be altered with this parameter.
  (0 will print all found decryptions.)

### xor

The program starts by separating the cipher into individual encrypted numbers.
There are two ways of doing this:

1. Search for characters that can't be decrypted (like spaces) and cut at those delimiters. <- requires delimiter
2. Ignore all of these possible delimiters and cut into numbers of equal length. <- requires encrypted numbers to be of equal length

The program will perform both of these methods (unless ordered not to), which might lead to the decrypted text being returned twice.

It will use all possible values for each setting unless otherwise specified.
These are the supported parameters:

- `-d <delimiters...>` or `--delim <delimiters...>`: Only these supplied delimiters will be used to split the encrypted numbers.
  The program won't attempt to separate the cipher with the length of the encrypted numbers, unless explicitly requested.
- `-l <lengths...>` or `--len <lengths...>`: Only these encrypted numbers lengths will be used.
  Analogue to the delimiters, all delimiters are getting ignored if not explicitly requested.
- `-b <bases...>` or `--base <bases...>`: If this parameter is used, only these bases will be tested.
- `-k <keys...>` or `--key <keys...>`: Only these keys will be tested if supplied.

- `-d <delimiters...>`
  console_arguments.add_optional({"-l", "--len"}, 1, -1);
  console_arguments.add_optional({"-b", "--base"}, 1, -1);
  console_arguments.add_optional({"-k", "--key"}, 1, -1);
  console_arguments.add_optional({"-t", "--transform"}, 1, -1);
  console_arguments.add_optional({"--algo"}, 1, -1);
  console_arguments.add_optional({"-a", "--amount"}, 1, 1);
  console_arguments.add_optional({"-f", "--file"}, 2, 2);

- plow
- transpose
- fence (using transpose with half the length of the cipher as key plus, rounded up)

### Example

```
./decryption "HloWrdel ol" --algo transform -t transpose -k 6
```

## [Utils](utils)

This is a statically linked library containing:

- an abstraction layer for console arguments,
- an implementation of the [Trie](https://en.wikipedia.org/wiki/Trie) data structure and
- generally useful preprocessor macros and functions.

### Command Line Parameters

The first parameter should be the cipher to be decrypted.
If the user has more information about the encryption used, she can use the following parameters:

- `-d <delimiters...>` or `--delim <delimiters...>`: Only these supplied delimiters will be used to split the encrypted numbers.
  The program won't attempt to separate the cipher with the length of the encrypted numbers, unless explicitly requested.
- `-l <lengths...>` or `--len <lengths...>`: Only these encrypted numbers lengths will be used.
  Analogue to the delimiters, all delimiters are getting ignored if not explicitly requested.
- `-b <bases...>` or `--base <bases...>`: If this parameter is used, only these bases will be tested.
- `-k <keys...>` or `--key <keys...>`: Only the these keys will be tested if supplied.
- `-f <file_path>` or `--file <file_path>`: When this flag is used, the output will also be stored in a file at `<file_path>` or "output.txt" if omitted.
- `-a <amount>` or `--amount <amount>`: The program returns the five best decryptions it can find.
  This amount can be altered with this parameter.
  (0 will print all found decryptions.)

# How to Build?

This repository uses [premake5](https://github.com/premake/premake-core/wiki) as its build system.
Therefore premake5 can be used to create e.g. Makefiles, Visual Studio .sln and .vcxproj files, etc...
Once the files for the compiler of your liking have been created, you can use that compiler to build the projects.

# Resources

- (english.txt)[https://github.com/LordHypersonic/C-Spell-Checker]
- (german1.txt)[https://sourceforge.net/projects/germandict/]
- (german2.txt)[https://sourceforge.net/p/germandict/code/HEAD/tree/variants.dic]
