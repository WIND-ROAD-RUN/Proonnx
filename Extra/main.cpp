#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    std::cout << "Current working directory: " << fs::current_path() << std::endl;
    return 0;
}