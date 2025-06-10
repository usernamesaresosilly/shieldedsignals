#include <vector>
#include <string>
#include <algorithm>
#include <optional>

#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>
#include <cryptopp/filters.h>

using namespace CryptoPP;

// forward-declare your init function (must be defined elsewhere)
void init_rsa_keys();

extern RSA::PublicKey public_key;
extern RSA::PrivateKey private_key;

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

        // cast to the correct byte-pointer type so the right overload is used:
        StringSource(
            reinterpret_cast<const byte*>(message.data() + offset),
            chunkSize,
            true,
            new PK_EncryptorFilter(rng, encryptor,
                new StringSink(chunkCT)
            )
        );

        ciphertexts.push_back(std::move(chunkCT));
    }

    return ciphertexts;
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

        StringSource(
            reinterpret_cast<const byte*>(message.data() + offset), // explicit cast to const byte*
            chunkSize,
            true,
            new PK_EncryptorFilter(rng, encryptor,
                new StringSink(chunkCT)
            )
        );

        ciphertexts.push_back(std::move(chunkCT));
    }

    return ciphertexts;
}

