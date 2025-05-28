#include "../rsa.h"
#include <iostream>
#include <string_view>

int main() {
    std::string_view cam_message =
        "CAM,StationID=101,Time=1713640000,Lat=52.5200,"
        "Lon=13.4050,Alt=34.2,Speed=13.4,Heading=92.3,Acc=0.5";

    auto ciphertext_opt = encrypt_string(std::string(cam_message));
    if (!ciphertext_opt) {
        std::cerr << "[rsa_test] Encryption failed!\n";
        return 1;
    }
    std::cout << "[rsa_test] Message encrypted successfully\n";

    auto decrypted_opt = decrypt_string(*ciphertext_opt);
    if (!decrypted_opt) {
        std::cerr << "[rsa_test] Decryption failed!\n";
        return 1;
    }
    std::cout << "[rsa_test] Decrypted message: " << *decrypted_opt << '\n';

    return 0;
}
