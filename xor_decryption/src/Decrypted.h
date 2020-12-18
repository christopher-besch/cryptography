#include <string>

class Decrypted
{
private:
    std::string decrypted_text;

public:
    Decrypted(std::string decrypted_text)
        : decrypted_text(decrypted_text) {}

    std::string get_text() { return decrypted_text; }
    virtual std::string get_type() = 0;
};

class XORDecrypted : public Decrypted
{
private:
    int key;
    int base;

public:
    XORDecrypted(std::string decrypted_text, int key, int base)
        : Decrypted(decrypted_text), key(key), base(base) {}

    virtual std::string get_type() override { return "XOR Cipher"; }
    int get_key() { return key; }
    int get_base() { return base; }
};
