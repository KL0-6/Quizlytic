#pragma once
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/md5.h>

#include <string>
#include <sstream>

namespace cryptography
{
	std::string hash(const std::string&, std::size_t, std::string = "sha384");
}