#include "cryptography.h"
#include <iomanip>
#include <c++/11/iomanip>

std::string cryptography::hash(const std::string& data, std::size_t data_size, std::string algorithm)
{
    const EVP_MD* algo = nullptr;

    // No string switch 😔
    if (algorithm == "md5") 
        algo = EVP_md5();
    else if (algorithm == "sha1") 
        algo = EVP_sha1();
    else if (algorithm == "sha224") 
        algo = EVP_sha224();
    else if (algorithm == "sha256") 
        algo = EVP_sha256();
    else if (algorithm == "sha384") 
        algo = EVP_sha384();
    else if (algorithm == "sha512") 
        algo = EVP_sha512();
    else if (algorithm == "sha3-224" || algorithm == "sha3_224")
        algo = EVP_sha3_224();
    else if (algorithm == "sha3-256" || algorithm == "sha3_256")
        algo = EVP_sha3_256();
    else if (algorithm == "sha3-384" || algorithm == "sha3_384") 
        algo = EVP_sha3_384();
    else if (algorithm == "sha3-512" || algorithm == "sha3_512") 
        algo = EVP_sha3_512();

    if (!algo)
        return "fail";

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx)
        return "fail";

    std::string result = "fail";

    if (EVP_DigestInit_ex(ctx, algo, NULL) == 1)
    {
        if (EVP_DigestUpdate(ctx, reinterpret_cast<const unsigned char*>(data.c_str()), data_size) == 1)
        {
            unsigned char hash[EVP_MAX_MD_SIZE];
            unsigned int hash_len = 0;

            if (EVP_DigestFinal_ex(ctx, hash, &hash_len) == 1)
            {
                std::stringstream ss;
                for (unsigned int i = 0; i < hash_len; ++i)
                    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);

                result = ss.str();
            }

            OPENSSL_cleanse(hash, sizeof(hash));
        }
    }

    EVP_MD_CTX_free(ctx);
    
    return result;
}