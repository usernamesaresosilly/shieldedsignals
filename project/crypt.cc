
#include "crypt.h"

#include <cryptopp/cryptlib.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>

#include <iostream>
#include <string>

using namespace CryptoPP;

// Fixed key and IV for testing
const byte FIXED_KEY[ AES::DEFAULT_KEYLENGTH ] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

const byte FIXED_IV[ AES::BLOCKSIZE ] = {
    0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08,
    0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00
};

// AES encryption
// input plaintext, returns encrypted ciphertext
std::string aes_encrypt(const std::string& plaintext) {
    std::string ciphertext;

    CBC_Mode< AES >::Encryption e;
    e.SetKeyWithIV(FIXED_KEY, sizeof(FIXED_KEY), FIXED_IV);

    StringSource s(plaintext, true, 
        new StreamTransformationFilter(e,
            new StringSink(ciphertext)
            )
    );

    return ciphertext;
}

// AES Decryption
// input ciphertext, returns plaintext
std::string aes_decrypt(const std::string& ciphertext) {
    std::string plaintext;

    CBC_Mode< AES >::Decryption d;
    d.SetKeyWithIV(FIXED_KEY, sizeof(FIXED_KEY), FIXED_IV);

    StringSource s(ciphertext, true, 
        new StreamTransformationFilter(d,
            new StringSink(plaintext)
        )
    );
    
    return plaintext;
}

