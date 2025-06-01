#include "rsa.h"
#include <cryptopp/rsa.h>
#include <cryptopp/osrng.h>
#include <cryptopp/pssr.h>
#include <cryptopp/filters.h>
#include <mutex>
#include <optional>

using namespace CryptoPP;

namespace {
    static RSA::PrivateKey private_key;
    static RSA::PublicKey public_key;
    static std::once_flag keys_once_flag;

    void do_init_rsa_keys() {
        AutoSeededRandomPool local_rng;
        private_key.GenerateRandomWithKeySize(local_rng, 2048);
        public_key.AssignFrom(private_key);
    }
}

void init_rsa_keys() {
    std::call_once(keys_once_flag, do_init_rsa_keys);
}

std::optional<std::string> rsa_encrypt(const std::string& message) {
    if (message.empty()) return std::nullopt;
    init_rsa_keys();
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Encryptor encryptor(public_key);
    size_t maxLen = encryptor.FixedMaxPlaintextLength();
    if (message.size() > maxLen) return std::nullopt;
    std::string ciphertext;
    try {
        StringSource(message, true,
            new PK_EncryptorFilter(rng, encryptor,
                new StringSink(ciphertext)
            )
        );
    } catch (const CryptoPP::Exception&) {
        return std::nullopt;
    }
    return ciphertext;
}

std::optional<std::string> rsa_decrypt(const std::string& ciphertext) {
    if (ciphertext.empty()) return std::nullopt;
    init_rsa_keys();
    AutoSeededRandomPool rng;
    RSAES_OAEP_SHA_Decryptor decryptor(private_key);
    std::string recovered;
    try {
        StringSource(ciphertext, true,
            new PK_DecryptorFilter(rng, decryptor,
                new StringSink(recovered)
            )
        );
    } catch (const CryptoPP::Exception&) {
        return std::nullopt;
    }
    return recovered;
}
