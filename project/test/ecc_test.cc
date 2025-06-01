#include "ecc.h"
#include <iostream>
#include <cassert>
#include <string>
#include <string_view>

int main() {
    std::string_view cam_message =
        "CAM,StationID=101,Time=1713640000,Lat=52.5200,"
        "Lon=13.4050,Alt=34.2,Speed=13.4,Heading=92.3,Acc=0.5";

    std::string plaintext(cam_message);  // convert to std::string

    // Test encryption
    auto encrypted = ecc_encrypt(plaintext);
    assert(encrypted.has_value());
    std::cout << "Encryption passed.\n";

    // Test decryption
    auto decrypted = ecc_decrypt(encrypted.value());
    assert(decrypted.has_value());
    std::cout << "Decryption passed.\n";

    // Check round-trip
    assert(plaintext == decrypted.value());
    std::cout << "Round-trip (encrypt->decrypt) passed.\n";

    // Test empty string encryption
    auto empty_encrypted = ecc_encrypt("");
    assert(!empty_encrypted.has_value());
    std::cout << "Empty message encryption returns nullopt as expected.\n";

    // Test decryption with invalid blob
    auto invalid_decrypted = ecc_decrypt("shortblob");
    assert(!invalid_decrypted.has_value());
    std::cout << "Invalid blob decryption returns nullopt as expected.\n";

    std::cout << "All ECC encryption/decryption tests passed.\n";
    return 0;
}
