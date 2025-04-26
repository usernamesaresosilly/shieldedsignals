/*
    -------------------------------------
    This file demonstrates simple encryption, evaluation, and decryption
    of a CAM-style message using homomorphic encryption (BFV scheme).

    Major Steps:
    - Encode a text message into a vector of integers
    - Encrypt the integers into SEAL ciphertexts
    - Perform operations (sum, difference, product, scaling, rotation) directly on encrypted data
    - Decrypt results and print them out

    Why this matters:
        Homomorphic encryption lets you process private data while keeping it private.
        We're hoping to integrate this with NS3 simulations eventually (wish us luck 🙃).

    Dependencies:
    - Microsoft SEAL library (https://github.com/microsoft/SEAL)

    How to run:
    - Compile this file with SEAL properly linked
    - The example will encrypt a sample CAM message, manipulate it homomorphically, and print results
*/

#include <cstdint>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <seal/seal.h>

namespace example {

// Struct to hold all SEAL components for easy reuse
struct SEALTools {
    seal::SEALContext context;
    seal::KeyGenerator keygen;
    seal::PublicKey publicKey;
    seal::SecretKey secretKey;
    seal::RelinKeys relinKeys;
    seal::GaloisKeys galoisKeys;
    seal::Encryptor encryptor;
    seal::Decryptor decryptor;
    seal::Evaluator evaluator;
    seal::BatchEncoder batchEncoder;

    // Factory method for initialization
    static SEALTools Create(
        size_t polyModulusDegree = 8192,
        int plainModulusBitSize = 20)
    {
        // Configure parameters
        seal::EncryptionParameters parms(seal::scheme_type::bfv);
        parms.set_poly_modulus_degree(polyModulusDegree);
        parms.set_coeff_modulus(
            seal::CoeffModulus::BFVDefault(polyModulusDegree));
        parms.set_plain_modulus(
            seal::PlainModulus::Batching(polyModulusDegree, plainModulusBitSize));

        SEALTools tools{
            seal::SEALContext(parms),
            seal::KeyGenerator(seal::SEALContext(parms)),
            {}, {}, {}, {},
            seal::Encryptor(seal::SEALContext(parms), seal::PublicKey{}),
            seal::Decryptor(seal::SEALContext(parms), seal::SecretKey{}),
            seal::Evaluator(seal::SEALContext(parms)),
            seal::BatchEncoder(seal::SEALContext(parms))
        };

        tools.keygen.create_public_key(tools.publicKey);
        tools.secretKey = tools.keygen.secret_key();
        tools.keygen.create_relin_keys(tools.relinKeys);
        tools.keygen.create_galois_keys(tools.galoisKeys);

        tools.encryptor = seal::Encryptor(tools.context, tools.publicKey);
        tools.decryptor = seal::Decryptor(tools.context, tools.secretKey);

        return tools;
    }
};

// Encrypt ASCII bytes of a message into a SEAL ciphertext
seal::Ciphertext encryptString(
    std::string_view message,
    SEALTools &tools)
{
    if (message.empty()) {
        throw std::invalid_argument("Input message cannot be empty");
    }

    std::vector<uint64_t> asciiValues;
    asciiValues.reserve(message.size());
    for (char c : message) {
        asciiValues.push_back(static_cast<uint64_t>(c));
    }

    seal::Plaintext plain;
    tools.batchEncoder.encode(asciiValues, plain);

    seal::Ciphertext ciphertext;
    tools.encryptor.encrypt(plain, ciphertext);
    return ciphertext;
}

// Decrypt SEAL ciphertext back to string
std::string decryptString(
    const seal::Ciphertext &cipher,
    SEALTools &tools)
{
    seal::Plaintext plainResult;
    tools.decryptor.decrypt(cipher, plainResult);

    std::vector<uint64_t> decoded;
    decoded.reserve(tools.batchEncoder.slot_count());
    tools.batchEncoder.decode(plainResult, decoded);

    std::string output;
    output.reserve(decoded.size());
    for (uint64_t code : decoded) {
        if (code > 0 && code < 256) {
            output.push_back(static_cast<char>(code));
        }
    }
    return output;
}

// Print decrypted ciphertext with label, catching errors
void dumpCiphertext(
    const seal::Ciphertext &cipher,
    std::string_view label,
    SEALTools &tools)
{
    try {
        std::string result = decryptString(cipher, tools);
        std::cout << label << ": " << result << '\n';
    } catch (const std::exception &e) {
        std::cerr << "Error decrypting " << label
                  << ": " << e.what() << '\n';
    }
}

// Run a full SEAL example: encrypt, manipulate, decrypt
// Operations performed:
//  - Sum of two encrypted messages
//  - Difference (should yield zero message)
//  - Product (square of ASCII codes)
//  - Scalar multiplication (all values doubled)
//  - Row rotation (shifted positions in ciphertext)
void runSealExample(
    std::string_view message,
    SEALTools &tools,
    int rotateSteps = 3,
    uint64_t scaleFactor = 2ULL)
{
    using namespace seal;

    auto ciphertext1 = encryptString(message, tools);
    auto ciphertext2 = encryptString(message, tools);

    Ciphertext sumCipher;
    tools.evaluator.add(ciphertext1, ciphertext2, sumCipher);

    Ciphertext diffCipher;
    tools.evaluator.sub(ciphertext1, ciphertext2, diffCipher);

    Ciphertext prodCipher;
    tools.evaluator.multiply(ciphertext1, ciphertext2, prodCipher);
    tools.evaluator.relinearize_inplace(prodCipher, tools.relinKeys);

    Plaintext scalePlain;
    std::vector<uint64_t> scaleVec(
        tools.batchEncoder.slot_count(), scaleFactor);
    tools.batchEncoder.encode(scaleVec, scalePlain);
    Ciphertext scaledCipher;
    tools.evaluator.multiply_plain(ciphertext1, scalePlain, scaledCipher);

    Ciphertext rotatedCipher;
    tools.evaluator.rotate_rows(
        ciphertext1, rotateSteps, tools.galoisKeys, rotatedCipher);

    std::cout << "Noise budget of ciphertext1: "
              << tools.decryptor.invariant_noise_budget(ciphertext1)
              << " bits\n";

    dumpCiphertext(sumCipher,    "Sum (twice)", tools);
    dumpCiphertext(diffCipher,   "Difference (empty)", tools);
    dumpCiphertext(prodCipher,   "Product (ASCII^2)", tools);
    dumpCiphertext(scaledCipher, "Scaled by " + std::to_string(scaleFactor), tools);
    dumpCiphertext(rotatedCipher,"Rotated by " + std::to_string(rotateSteps), tools);
}

} // namespace example

int main()
{
    // placeholder usage
    const std::string_view camMessage =
        "CAM,StationID=101,Time=1713640000,Lat=52.5200,"
        "Lon=13.4050,Alt=34.2,Speed=13.4,Heading=92.3,Acc=0.5";

    auto tools = example::SEALTools::Create();
    example::runSealExample(camMessage, tools);

    return 0;
}

