#ifndef RSA_CHUNKER_H
#define RSA_CHUNKER_H

#include <vector>
#include <string>
#include <optional>

#include <cryptopp/rsa.h>

// Initializes the RSA keypair exactly once
void init_rsa_keys();

// The RSA keypair (defined in rsa.cpp)
extern CryptoPP::RSA::PrivateKey private_key;
extern CryptoPP::RSA::PublicKey  public_key;

// Encrypts a potentially-large message by splitting it into OAEP-sized chunks
// Returns an empty vector on failure
std::vector<std::string> rsa_encrypt_chunks(const std::string& message);

// Decrypts the vector of RSA-encrypted chunks and reassembles the original
// Returns std::nullopt on any failure
std::optional<std::string> rsa_decrypt_chunks(const std::vector<std::string>& ciphertexts);

#endif // RSA_CHUNKER_H