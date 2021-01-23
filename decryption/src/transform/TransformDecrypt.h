#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "Utils.h"
#include "Transformation.h"

#include "goodness/LibrarySearch.h"
#include "Decrypted.h"

// for one valid decryption option
struct TransformDecrypted : public Decrypted
{
    int key;
    std::string transformation_type;

    TransformDecrypted() {}
    TransformDecrypted(std::string decrypted_str, int key, std::string transformation_type)
        : Decrypted(decrypted_str, score), key(key), transformation_type(transformation_type) {}

    // todo: better alignment
    virtual void create_report() override
    {
        std::stringstream report_ss;
        report_ss << decrypted_str << "\t-" << transformation_type << " Transformation, Key " << key << ", Score " << score;
        report = report_ss.str();
    }
};

// for one cipher and all valid decryption options
class TransformDecryptor : public Decryptor
{
private:
    std::vector<TransformDecrypted> m_decryptions;

    std::vector<int> m_requested_keys;
    std::vector<std::string> m_requested_transformation_type;

private:
    // when a certain setting is requested, everything else won't be tested
    bool is_to_test_key(int key)
    {
        return m_requested_keys.empty() || std::find(m_requested_keys.begin(), m_requested_keys.end(), key) != m_requested_keys.end();
    }
    bool is_to_test_transformation_type(std::string transformation_type)
    {
        return m_requested_transformation_type.empty() ||
               std::find(m_requested_transformation_type.begin(), m_requested_transformation_type.end(), transformation_type) != m_requested_transformation_type.end();
    }

    void test_decryptions(transformation_func transformation, TransformDecrypted &template_decrypt, bool row_count_known);

public:
    TransformDecryptor(const std::string &cipher, const LibrarySearch &dictionary)
        : Decryptor(cipher, dictionary) {}
    // copies are not allowed
    TransformDecryptor(const TransformDecryptor &) = delete;
    TransformDecryptor &operator=(const TransformDecryptor &) = delete;

    void add_requested_key(int key)
    {
        if (key < 1 || key > m_cipher.size())
            raise_error("The provided key '" << key << "' is invalid!");
        m_requested_keys.push_back(key);
    }
    void add_requested_transformation_types(std::string transformation_type)
    {
        if (transformation_type != "plow" && transformation_type != "transpose")
            raise_error("The provided transformation type '" << transformation_type << "' is invalid!");
        m_requested_transformation_type.push_back(transformation_type);
    }

    std::vector<TransformDecrypted> &get_decryptions() { return m_decryptions; }

    virtual void create_decryptions(int amount = 0) override;
};
