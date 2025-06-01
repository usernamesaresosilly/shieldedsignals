
#ifndef RSA_H
#define RSA_H

#include <iostream>
#include <string>
#include <optional>

std::optional<std::string> rsa_encrypt(const std::string& message);

std::optional<std::string> rsa_decrypt(const std::string& ciphertext);

#endif