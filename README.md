# 🔒Cryptography

This repository contains two console applications, `encryption` and `decryption`.
Both programs are designed to be executed from the command line.

## Encryption

`encryption` encrypts the input using one of the supported algorithms.
To select the desired algorithm, the parameter `-a <algorithm name>` or `--algorithm <algorithm name>` shall be used.
Each algorithms uses certain settings/keys.
If a value for one of these settings is supplied as a console line parameter, only this value for this setting will be used.
If the setting is not supplied, all possible values for this setting will be used in succession, returning multiple different ciphers.

The input text can be supplied as the first parameter (otherwise the user has to input it later on).

### Algorithms

- `xor`:
  - `-k <key>` or `--key <key>`: each number (representing a single character using ASCII) get xored with this number
  - `-b <base>` or `--base <base>`: sets base, every encrypted number shall be represented in
  - `-d <delimiter>` or `--delim <delimiter>`: if not supplied, the encrypted numbers won't be separated, if the `<delimiter>` is omitted, a space will be used
  - `-o`: when set, each encrypted numbers will have the length (0s will be used as a padding)
- `plow`:
  - `-k <key>`: sets the width of the array, the input gets read into
- `transpose`:
  - `-k <key>`: sets the width of the array, the input gets read into
- `fence`

### Example

These examples are from linux, windows users have to replace `./encryption` with `encryption.exe`.

```
./encryption "Hello World!" -a xor -b 16 -k 42 -0 -d
```

This command returns the cipher `62 4f 46 46 45 0a 7d 45 58 46 4e 0b` using the xor encryption algorithm with the key 42. The encrypted characters are represented in base-16 (hex), separated with spaces and padded with 0s.

```
./encryption "Hello World!" -a xor -b 16 -0 -d
```

This command does the same as the one prior with the difference that all possible keys (0-255) are used in succession.

## Decryption

`decryption` is able to decrypt certain ciphers automatically.
The user gives the program as much information as she has and it tries its best to decrypt the entered cipher.
The program uses brute force by default, creating all decryptions using all possible settings and decryption algorithms.
Each decrypted string gets a score based on its human comprehensibility.
To determine the score of a given string, a dictionary of English and German words is being used.
The user is able to add custom words into `user_dict.dic` in the `resource` folder.
Thus anything that can't be found in the dictionaries, like links or telephone numbers, will receive bad scores.
That's why this program won't be able to automatically decrypt these, manual decryption is necessary in this case.
The user can use the `-m 0` parameter and redirect the output into a file to achieve that.

The input cipher can be supplied as the first parameter (otherwise the user has to input it later on).

These parameters are supported with all algorithms:

- `-a <algorithms...>` or `--algorithm <algorithms...>`: This can be set to `xor` or `transform` each for one of the following decryption algorithms.
- `-m <amount>` or `--amount <amount>`: By default the program returns the best decryptions it can find.
  With this parameter only a certain amount of decryptions will be generated.
  (0 will print all found decryptions.)
- `-f <input_file_path> <output_file_path>` or `--file <input_file_path> <output_file_path>`: When this flag is used, each line from `<input_file_path>` will be treated as an individual cipher and decrypted into `<output_file_path>`.
  Every line can use a different algorithm / settings.

### XOR

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

### Transformation

This implements multiple decryption algorithms (plow, transpose and fence).

The cipher gets read into a 2D array row by row.
The widths of the array that shall be tested can be specified with `-k <keys...>` or `--key <keys...>`.
Each algorithm is characterized by a certain transformation used on the 2D array.
Once that is done, the transformed 2D array gets read row by row.

The used transformation can be specified with `-t <transformations...>` or `--transform <transformations...>`.
`plow` and `transpose` are supported.

Fence decryption can be achieved by using a transpose transformation with half the length of the cipher as key plus, rounded up.

### Example

These examples are from linux, windows users have to replace `./decryption` with `decryption.exe`.

```
./decryption "HloWrdel ol"
```

In this case the user doesn't know anything about the used encryption process, thus no parameters are supplied.

```
./decryption "HloWrdel ol" -a transform -t transpose -k 6
```

This cipher has been created using the fence encryption and has the length 11.
Therefore the transform algorithm with the transpose transformation can be used with the key 6.

# Download

The latest build for both linux and windows can be downloaded [here](https://github.com/christopher-besch/cryptography/releases/latest).

## How to Build?

This repository uses [premake5](https://github.com/premake/premake-core/wiki) as its build system.
Therefore premake5 can be used to create e.g. Makefiles, Visual Studio .sln and .vcxproj files, etc...
Once the files for the compiler of your liking have been created, you can use that compiler to build the projects.

# Implementation Details

This repository consists of four projects], [encryption](encryption), [decryption](decryption), [utils](utils) and [test](test).

## utils

This is a statically linked library containing:

- an abstraction layer for console arguments,
- an implementation of the [Trie](https://en.wikipedia.org/wiki/Trie) data structure
- string transformation functionality used for both encrypting and decrypting transformation ciphers and
- generally useful preprocessor macros and functions.

## test

This repository is under constant development.
New algorithms get tested in this project.

# Resources

- [english.dic](https://github.com/LordHypersonic/C-Spell-Checker)
- [german1.dic](https://sourceforge.net/projects/germandict/)
- [german2.dic](https://sourceforge.net/p/germandict/code/HEAD/tree/variants.dic)
