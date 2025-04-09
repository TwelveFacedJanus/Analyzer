import numbers_common;  // Changed from import to include
import application;

#include <iostream>

int main() {
    auto result = make_ok(42);
    
    std::cout << APPLICATION_NAME << " v"
              << APPLICATION_MAJOR_VERSION << "."
              << APPLICATION_MINOR_VERSION << "."
              << APPLICATION_PATCH_VERSION << "\n";
              
    std::cout << "Result: " << result.unwrap() << "\n";
    return 0;
}