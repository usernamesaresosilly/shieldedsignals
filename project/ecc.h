#pragma once
#include <string>
#include <optional>

// Encrypts a string using ephemeral ECDH + AES-CBC.
// Returns output blob or std::nullopt on error.
std::optional<std::string> encrypt_string(const std::string& message);

// Decrypts a string produced by encrypt_string.
// Returns plaintext or std::nullopt on error.
std::optional<std::string> decrypt_string(const std::string& blob);
