#include "unitest.hpp"
#include "utilities.hpp"
#include <iostream>
#if UNITEST_TESTS_FILE_OUTPUT
    #include <fstream>
#endif
    
using namespace unitest;

void TestWithFunctionPointer_Passes(const Test& assert) {
    assert.is_true(true);
    assert.is_false(false);
}

int main() {
    Runner tests {};
    
#if UNITEST_TESTS_FILE_OUTPUT
    constexpr const char output_file[] { "./output.txt" };

    std::ofstream file { output_file };

    if (!file.is_open()) {
        std::cerr << "ERROR - Could not open " << output_file << '\n';
        return 1;
    }

    tests.set_output(&file);
#endif
    
    // Adding tests that use a raw function pointer instead of a lambda.
    tests.add({
        "TestWithFunctionPointer_Passes",
        "passes",
        TestWithFunctionPointer_Passes
    });

    tests.add({
        "add",
        "adds two numbers",
        [](const Test& assert) {
            constexpr int x { 10 };
            constexpr int y { 8 };
            
            const int expected { x + y };
            const int actual { tests::add(x, y) };

            assert.are_equal(expected, actual);
            assert.is_true(expected == actual);
        }
    });

    tests.add({
        "sub",
        "subtracts two numbers",
        [](const Test& assert) {
            constexpr int x { 10 };
            constexpr int y { 8 };

            const int expected { (x - y) + 1 };
            const int actual { tests::sub(x, y) };

            assert.are_not_equal(expected, actual);
            assert.is_false(expected == actual);
        }
    });

    tests.add({
        "mul",
        "multiplies one number by another",
        [](const Test& assert) {
            assert.throws_no_exception<std::runtime_error>([]() { tests::mul(10, 5); });
        }
    });
    
    tests.add({
        "div",
        "throws exception if attempting to divide by zero",
        [](const Test& assert) {
            assert.throws_exception<std::runtime_error>([]() { tests::div(10, 0); });
        }
    });

    // We intentionally allow this to fail.
    tests.add({
        "mod",
        "does not throw exception if attempting to divide by zero",
        [](const Test& assert) {
            // These extra initial asserts are used to show that the assertion count per-test is
            //     being incremented.
            assert.is_true(true);
            assert.is_false(false);

            assert.throws_no_exception<std::runtime_error>([]() { tests::mod(10, 0); });
        }
    });

    // Testing custom assertion failure messages.
    tests.add({
        "div",
        "throws an exception if attempting to divide by zero",
        [](const Test& assert) {
            assert.throws_exception<std::runtime_error>([]() {
                tests::mod(10, 5); },
                "LOL, assertion failure"
            );
        }
    });

    // This supposed to fail.
    tests.add({
        "boolean test",
        "fails",
        [](const Test& assert) {
            assert.is_true(false);
        }
    });

    tests.add(tests::get_entity_tests());

    tests.run();

    return 0;
}
