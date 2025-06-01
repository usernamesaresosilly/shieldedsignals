#include "ecc.h"
#include <cryptopp/eccrypto.h>
#include <cryptopp/osrng.h>
#include <cryptopp/oids.h>
#include <cryptopp/secblock.h>
#include <cryptopp/aes.h>
#include <cryptopp/modes.h>
#include <cryptopp/filters.h>
#include <mutex>
#include <string>
#include <cstring>

using namespace CryptoPP;

namespace {
    const ECDH<ECP>::Domain& get_ec_domain() {
        static ECDH<ECP>::Domain domain(ASN1::secp256r1());
        return domain;
    }

    static std::once_flag keys_once_flag;
    static SecByteBlock privateKey, publicKey;

    void do_init_keys() {
        AutoSeededRandomPool rng;
        const auto& domain = get_ec_domain();

        privateKey.CleanNew(domain.PrivateKeyLength());
        publicKey.CleanNew(domain.PublicKeyLength());
        domain.GenerateKeyPair(rng, privateKey, publicKey);
    }

    void ensure_keys_initialized() {
        std::call_once(keys_once_flag, do_init_keys);
    }
}

std::optional<std::string> ecc_encrypt(const std::string& message) {
    if (message.empty()) return std::nullopt;
    ensure_keys_initialized();

    AutoSeededRandomPool rng;
    const auto& domain = get_ec_domain();

    SecByteBlock ephPriv(domain.PrivateKeyLength()), ephPub(domain.PublicKeyLength());
    domain.GenerateKeyPair(rng, ephPriv, ephPub);

    SecByteBlock shared(domain.AgreedValueLength());
    if (!domain.Agree(shared, ephPriv, publicKey)) return std::nullopt;

    SecByteBlock aesKey(AES::DEFAULT_KEYLENGTH);
    std::memcpy(aesKey.data(), shared.data(), aesKey.size());

    byte iv[AES::BLOCKSIZE];
    rng.GenerateBlock(iv, sizeof(iv));

    std::string ciphertext;
    try {
        CBC_Mode<AES>::Encryption aesEnc;
        aesEnc.SetKeyWithIV(aesKey, aesKey.size(), iv);
        StringSource(message, true,
            new StreamTransformationFilter(aesEnc, new StringSink(ciphertext))
        );
    } catch (...) {
        return std::nullopt;
    }

    std::string output;
    output.append(reinterpret_cast<const char*>(ephPub.data()), ephPub.size());
    output.append(reinterpret_cast<const char*>(iv), sizeof(iv));
    output.append(ciphertext);
    return output;
}

std::optional<std::string> ecc_decrypt(const std::string& blob) {
    ensure_keys_initialized();
    const auto& domain = get_ec_domain();

    const size_t ephLen = domain.PublicKeyLength();
    const size_t ivLen = AES::BLOCKSIZE;
    if (blob.size() < ephLen + ivLen) return std::nullopt;

    SecByteBlock ephPub(reinterpret_cast<const byte*>(blob.data()), ephLen);
    const byte* iv = reinterpret_cast<const byte*>(blob.data() + ephLen);
    std::string cipherText = blob.substr(ephLen + ivLen);

    SecByteBlock shared(domain.AgreedValueLength());
    if (!domain.Agree(shared, privateKey, ephPub)) return std::nullopt;

    SecByteBlock aesKey(AES::DEFAULT_KEYLENGTH);
    std::memcpy(aesKey.data(), shared.data(), aesKey.size());

    std::string recovered;
    try {
        CBC_Mode<AES>::Decryption aesDec;
        aesDec.SetKeyWithIV(aesKey, aesKey.size(), iv);
        StringSource(cipherText, true,
            new StreamTransformationFilter(aesDec, new StringSink(recovered))
        );
    } catch (...) {
        return std::nullopt;
    }

    return recovered;
}
