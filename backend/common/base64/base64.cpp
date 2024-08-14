#include "base64.h"

std::string base64::encode(const std::string& data, std::size_t size)
{
    BIO* b64 = BIO_new(BIO_f_base64());
    if (!b64)
        throw std::runtime_error("Failed to create BIO for Base64 encoding");

    BIO* bio  = BIO_new(BIO_s_mem());
    if (!bio)
    {
        BIO_free(b64);
            
        throw std::runtime_error("Failed to create BIO for memory buffer");
    }

    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    if (BIO_write(bio, data.data(), size) <= 0 || BIO_flush(bio) <= 0)
    {
        BIO_free_all(bio);

        throw std::runtime_error("Failed to write data to BIO");
     }

    BUF_MEM *bufferPtr = nullptr;
    BIO_get_mem_ptr(bio, &bufferPtr);
    if (!bufferPtr || !bufferPtr->data || bufferPtr->length == 0)
    {
        BIO_free_all(bio);
            
        throw std::runtime_error("Failed to get memory buffer from BIO");
    }

    std::string encodedData(bufferPtr->data, bufferPtr->length);

    BIO_free_all(bio);

    return encodedData;
}

std::string base64::decode(const std::string& encodedData)
{
    BIO* b64 = BIO_new(BIO_f_base64());
    if (!b64)
        throw std::runtime_error("Failed to create BIO for Base64 decoding");

    BIO* bio  = BIO_new(BIO_s_mem());
    if (!bio)
    {
        BIO_free(b64);
            
        throw std::runtime_error("Failed to create BIO for memory buffer");
    }

    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    std::size_t encodedLen = encodedData.length();
    std::string decodedData(encodedLen, '\0');

    int len = BIO_read(bio, &decodedData[0], encodedLen);
    if (len < 0)
    {
        BIO_free_all(bio);

        throw std::runtime_error("Failed to read data from BIO");
    }

    decodedData.resize(len);

    BIO_free_all(bio);
    
    return decodedData;
}