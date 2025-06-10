#include <vector>
#include <string>
#include <algorithm>
#include <optional>

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>
#include <cryptopp/filters.h>

using namespace CryptoPP;

/// define the globals exactly once
CryptoPP::RSA::PrivateKey private_key;
CryptoPP::RSA::PublicKey  public_key;

void init_rsa_keys() {
    static bool inited = false;
    if (!inited) {
        CryptoPP::AutoSeededRandomPool rng;
        private_key.GenerateRandomWithKeySize(rng, 2048);
        public_key.Initialize(private_key.GetModulus(), private_key.GetPublicExponent());
        inited = true;
    }
}

std::vector<std::string> rsa_encrypt_chunks(const std::string& message) {
    init_rsa_keys();
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Encryptor encryptor(public_key);

    size_t maxLen = encryptor.FixedMaxPlaintextLength();
    std::vector<std::string> ciphertexts;
    ciphertexts.reserve((message.size() + maxLen - 1) / maxLen);

    for (size_t offset = 0; offset < message.size(); offset += maxLen) {
        size_t chunkSize = std::min(maxLen, message.size() - offset);
        std::string chunkCT;

        // cast char* -> byte* so it matches the StringSource overload
        StringSource(
            reinterpret_cast<const byte*>(message.data() + offset),
            chunkSize,
            true,  // pumpAll
            new PK_EncryptorFilter(
                rng,
                encryptor,
                new StringSink(chunkCT)
            )
        );

        ciphertexts.push_back(std::move(chunkCT));
    }

    return ciphertexts;
}

std::optional<std::string> rsa_decrypt_chunks(const std::vector<std::string>& ciphertexts) {
    init_rsa_keys();
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Decryptor decryptor(private_key);

    std::string recovered;
    for (const auto& chunkCT : ciphertexts) {
        // similarly cast here
        StringSource(
            reinterpret_cast<const byte*>(chunkCT.data()),
            chunkCT.size(),
            true,
            new PK_DecryptorFilter(
                rng,
                decryptor,
                new StringSink(recovered)
            )
        );
    }

    return recovered;
}
