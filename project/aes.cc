#include "crypt.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/secblock.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

#include <iostream>
#include <string>

using namespace CryptoPP;

// Global in-memory key and IV
static SecByteBlock g_aesKey(24);              // 24 bytes = 192 bits
static SecByteBlock g_aesIV(AES::BLOCKSIZE);   // 16 bytes

// Initialize key/IV once
void init_aes_key()
{
    static bool inited = false;
    if (inited) return;
    AutoSeededRandomPool rng;

    // Generate a random 192-bit key
    rng.GenerateBlock(g_aesKey, g_aesKey.size());
    // Generate a random IV
    rng.GenerateBlock(g_aesIV,   g_aesIV.size());

    inited = true;
}

// AES-192 CBC encryption
std::string aes_encrypt(const std::string& plaintext)
{
    init_aes_key();

    std::string ciphertext;
    CBC_Mode< AES >::Encryption enc;
    // Pass in the 192-bit key (24 bytes)
    enc.SetKeyWithIV(g_aesKey, g_aesKey.size(), g_aesIV);

    StringSource ss(plaintext, true,
        new StreamTransformationFilter(enc,
            new StringSink(ciphertext)
        ) // StreamTransformationFilter
    ); // StringSource

    return ciphertext;
}

// AES-192 CBC decryption
std::string aes_decrypt(const std::string& ciphertext)
{
    init_aes_key();

    std::string recovered;
    CBC_Mode< AES >::Decryption dec;
    dec.SetKeyWithIV(g_aesKey, g_aesKey.size(), g_aesIV);

    StringSource ss(ciphertext, true,
        new StreamTransformationFilter(dec,
            new StringSink(recovered)
        )
    );
    return recovered;
}
