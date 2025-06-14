/*
    -------------------------------------
    This file demonstrates encryption and decryption
    of a CAM-style message using homomorphic encryption (BFV scheme).

    Major Changes From Older Versions:
    - SEAL keys, encryption parameters, and context are now static (singleton pattern)
    - Encryptor/Decryptor/Evaluator/BatchEncoder are initialized once, shared globally
    - Compatible with NS3 simulations where multiple components must share a static key setup
    - Simplified encrypt and decrypt functions for easy calling from any simulation module

    Why this matters:
        Homomorphic encryption lets you process private data while keeping it private.
        By making the SEAL keys and context static, this design ensures
        NS3 nodes and applications can reuse encryption without rebuilding state,
        allowing encrypted data to be passed reliably in simulations.

    Internal Design Overview:
                  (at startup)
                        ↓
          ┌──────────────────────────────┐
          │    static SEALSingleton       │
          │  (holds keys, context, tools) │
          └──────────────────────────────┘
                        ↓
      encrypt_string() uses singleton.encryptor
      decrypt_string() uses singleton.decryptor
                        ↓
  packets, nodes, apps can call it freely during simulation

    Dependencies:
    - Microsoft SEAL library (https://github.com/microsoft/SEAL)

    How to run:
    - Compile this file with SEAL properly linked
    - The example will encrypt a sample CAM message, decrypt it, and print the result
*/
#include "he.h"
#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <memory>
#include <seal/seal.h>

namespace example {
namespace {

    // Static Singleton containing all SEAL components
    struct SEALSingleton {
        seal::EncryptionParameters parms;
        std::shared_ptr<seal::SEALContext> context;
        std::unique_ptr<seal::KeyGenerator> keygen;
        seal::PublicKey public_key;
        seal::SecretKey secret_key;
        seal::RelinKeys relin_keys;
        seal::GaloisKeys galois_keys;
        std::unique_ptr<seal::Encryptor> encryptor;
        std::unique_ptr<seal::Decryptor> decryptor;
        std::unique_ptr<seal::Evaluator> evaluator;
        std::unique_ptr<seal::BatchEncoder> batch_encoder;

        SEALSingleton() {
            parms = seal::EncryptionParameters(seal::scheme_type::bfv);
            parms.set_poly_modulus_degree(8192);
            parms.set_coeff_modulus(seal::CoeffModulus::BFVDefault(8192));
            parms.set_plain_modulus(seal::PlainModulus::Batching(8192, 20));

            context = std::make_shared<seal::SEALContext>(parms);

            keygen = std::make_unique<seal::KeyGenerator>(*context);
            keygen->create_public_key(public_key);
            secret_key = keygen->secret_key();
            keygen->create_relin_keys(relin_keys);
            keygen->create_galois_keys(galois_keys);

            encryptor = std::make_unique<seal::Encryptor>(*context, public_key);
            decryptor = std::make_unique<seal::Decryptor>(*context, secret_key);
            evaluator = std::make_unique<seal::Evaluator>(*context);
            batch_encoder = std::make_unique<seal::BatchEncoder>(*context);
        }
    };

    SEALSingleton &singleton() {
        static SEALSingleton inst;
        return inst;
    }

} // namespace

seal::Ciphertext encrypt_string(std::string_view msg) {
    if (msg.empty()) throw std::invalid_argument("Input message is empty");

    std::vector<uint64_t> ascii_values;
