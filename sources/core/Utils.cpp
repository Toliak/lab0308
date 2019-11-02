#include <string>
#include <random>

#include "Utils.h"


std::string generateRandomLogin()
{
    static const size_t LENGTH = 12;
    static const std::string CHARS = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM_1234567890";

    static std::mt19937 generator{std::random_device{}()};
    static std::uniform_int_distribution<size_t> getCharIndex{0, CHARS.size() - 1};

    std::string result;
    for (size_t i = 0; i < LENGTH; i++) {
        result += CHARS[getCharIndex(generator)];
    }

    return result;
}
