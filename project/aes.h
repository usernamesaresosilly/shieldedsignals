
#ifndef CRYPT_H
#define CRYPT_H

#include <string>

std::string aes_encrypt(const std::string& plaintext);

std::string aes_decrypt(const std::string& ciphertext);

#endif