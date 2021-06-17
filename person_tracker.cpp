#include <iostream>
#include "utils/Utils.h"
#include <fmt/format.h>

int main() {
    std::cout << "------------\n";
    std::cout << "Hello World!" << '\n';
    std::cout << "------------\n";

    std::string myString = "World";
    Utils::print(fmt::format("Hello {} (from Utils with fmt)", myString));
    return 0;
}
