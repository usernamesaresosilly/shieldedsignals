#include "cam_generation.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

int main(int argc, char* argv[]) {
    int count = 1; // Default to 1 message

    if (argc > 1) {
        if (std::strcmp(argv[1], "--help") == 0) {
            std::printf("Usage: %s [number_of_messages]\n", argv[0]);
            return 0;
        }

        char* end = nullptr;
        count = std::strtol(argv[1], &end, 10);
        if (!end || *end != '\0' || count <= 0) {
            std::fprintf(stderr, "Error: Argument must be a positive integer.\n");
            return 1;
        }
    }

    std::string output = generate_messages(count);
    std::fputs(output.c_str(), stdout);

    return 0;
}
