# 🔒Cryptography

This repository contains three projects:

## [Utils](utils)

This is a statically linked library containing:

- an abstraction layer for console arguments,
- an implementation of the [Trie](https://en.wikipedia.org/wiki/Trie) data structure and
- generally useful preprocessor macros and functions.

## [Encryption](encryption)

Using this console application the user is able to encrypt texts with the currently supported encryption algorithms:

- XOR

### Command Line Parameters

The text that shall be encrypted can supplied as the first parameter (otherwise the user has to input it later on).
When no other parameters are given, the program will create all encryptions with all possible xor keys (0 to 255) and bases (2 to 36).
These parameters can be used to more closely tweak the parameters used for the encryption(s):

- `-b <bases>` or `--base <bases>`: Only the base `<base>` will be used with all keys.
- `-k <keys>` or `--key <keys>`: All bases will be used with the key `<key>`.
- `-d <delimiter>` or `--delim <delimiter>`: When this parameter is used, the different encrypted numbers representing the input text will be separated by `<delimiter>`.
  If `<delimiter>` is omitted, spaces will be used.
- `-0`: this flag will add zeros to every encrypted number so that they are all of the same length.

## [Decryption](decryption)

This is a console application able to decrypt certain ciphers automatically.
The user gives the program as much information as she has and it tries its best to decrypt the entered cipher.

To achieve that, the program starts by separating the cipher into individual encrypted numbers.
There are two ways of doing this:

1. Search for characters that can't be decrypted (like spaces) and cut at those delimiters.
2. Ignore all of these possible delimiters and cut into numbers of equal length.

The program will perform both of these methods which might lead to the decrypted text being returned twice.

### Command Line Parameters

The first parameter should be the cipher to be decrypted.
If the user has more information about the encryption used, she can use the following parameters:

- `-d <delimiters...>` or `--delim <delimiters...>`: Only these supplied delimiters will be used to split the encrypted numbers.
  The program won't attempt to separate the cipher with the length of the encrypted numbers, unless explicitly requested.
- `-l <lengths...>` or `--len <lengths...>`: Only these encrypted numbers lengths will be used.
  Analogue to the delimiters, all delimiters are getting ignored if not explicitly requested.
- `-b <bases...>` or `--base <bases...>`: If this parameter is used, only these bases will be tested.
- `-k <keys...>` or `--key <keys...>`: Only the these keys will be tested if supplied.
- `-f <file_path>` or `--file <file_path>`: When this flag is used, the output will also be stored in a file at `<file_path>` or "output.txt" if not given.
- `-a <amount>` or `--amount <amount>`: The program returns the five best decryptions it can find.
  This amount can be altered with this parameter.
  (0 will print all found decryptions.)

# How to build?

This repository uses [premake5](https://github.com/premake/premake-core/wiki) as its build system.
Therefore premake5 can be used to e.g. create Makefiles, Visual Studio .sln and .vcxproj files, etc...
Once the files for the compiler of your liking have been created, you can use that compiler to build the projects.
