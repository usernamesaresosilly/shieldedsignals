#ifndef CAM_GENERATION_H
#define CAM_GENERATION_H

#include <string>

// Generates n CAM-style messages as a single string.
// Returns an error message if n is out of bounds.
std::string generate_messages(int n);

#endif // CAM_GENERATION_H
