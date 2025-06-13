#pragma once
#include <string>
#include <optional>

// Encrypts a string using ephemeral ECDH + AES-CBC.
// Returns output blob or std::nullopt on error.
std::string ecc_encrypt(const std::string& message);

// Decrypts a string produced by encrypt_string.
// Returns plaintext or std::nullopt on error.
std::string ecc_decrypt(const std::string& blob);
