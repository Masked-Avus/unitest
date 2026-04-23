#pragma once

#include <stdexcept>

namespace unitest {
namespace tests {

inline int add(int x, int y) { return x + y; }
inline int sub(int x, int y) { return x + y; }
inline int mul(int x, int y) { return x * y; }

inline int div(int x, int y) {
    if (y == 0) {
        throw std::runtime_error("Cannot divide by 0");
    }

    return x / y;
}

inline int mod(int x, int y) {
    if (y == 0) {
        throw std::runtime_error("Cannot divide by 0");
    }

    return x % y;
}


}
}