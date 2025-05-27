#pragma once
#include <seal/seal.h>
#include <string>
#include <string_view>

namespace example {
seal::Ciphertext encrypt_string(std::string_view msg);
std::string decrypt_string(const seal::Ciphertext &cipher);
} // namespace example
