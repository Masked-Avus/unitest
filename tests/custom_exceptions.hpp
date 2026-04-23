#pragma once

#include <string>

namespace unitest {
namespace tests {

class Custom_Base_Exception {
public:
    Custom_Base_Exception(const char* message) : m_message { message } { }

    const char* what() const { return m_message.data(); }

private:
    std::string m_message {};
};

void throw_custom_base_exception() {
    throw Custom_Base_Exception("BASE EXCEPTION");
}

class Custom_Derived_Exception final : public Custom_Base_Exception {
public:
    Custom_Derived_Exception(const char* message) : Custom_Base_Exception { message } { }
};

void throw_custom_derived_exception() {
    throw Custom_Derived_Exception("DERIVED EXCEPTION");
}

}
}
