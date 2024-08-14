#include <openssl/evp.h>
#include <openssl/buffer.h>

#include <string>
#include <stdexcept>

namespace base64
{
    std::string encode(const std::string&, std::size_t);
    std::string decode(const std::string&);
}