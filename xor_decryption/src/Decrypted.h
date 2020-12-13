#include <string>

class Decrypted
{
private:
    std::string text;

public:
    Decrypted(std::string text)
        : text(text) {}

    std::string get_text() { return text; }

    virtual std::string get_type() = 0;
};

class XORDecrypted : public Decrypted
{
private:
    int key;
    int base;

public:
    XORDecrypted(std::string text, int key, int base)
        : Decrypted(text), key(key), base(base) {}

    virtual std::string get_type() override { return "XOR Cipher"; }
};
