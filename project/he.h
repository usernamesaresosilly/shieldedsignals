/*
-------------------------------------
    SEAL Encryption/Decryption Interface for NS3 Simulations

    - Provides easy encrypt_string() and decrypt_string() APIs
    - Internally uses a static singleton to store SEAL context and keys
    - Designed for safe reuse across packets, nodes, and events
    - Homomorphic encryption based on BFV scheme (Microsoft SEAL)

    Dependencies:
    - Microsoft SEAL library (https://github.com/microsoft/SEAL)
*/

#pragma once

#include <string>
#include <string_view>
#include <seal/seal.h>

namespace example {

    // Encrypts an ASCII message into a SEAL ciphertext
    seal::Ciphertext encrypt_string(std::string_view msg);

    // Decrypts a SEAL ciphertext back into a string
    std::string decrypt_string(const seal::Ciphertext &cipher);

} // namespace example
