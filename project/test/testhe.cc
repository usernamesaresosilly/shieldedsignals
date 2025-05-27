#include <iostream>
#include "../he.h"

int main() {
    using namespace example;

    std::string_view cam_message =
        "CAM,StationID=101,Time=1713640000,Lat=52.5200,"
        "Lon=13.4050,Alt=34.2,Speed=13.4,Heading=92.3,Acc=0.5";

    auto ciphertext = encrypt_string(cam_message);
    std::cout << "Message encrypted successfully\n";

    auto decrypted = decrypt_string(ciphertext);
    std::cout << "Decrypted message: " << decrypted << '\n';

    return 0;
}
