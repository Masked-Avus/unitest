#include "unitest.hpp"
#include "utilities.hpp"
#include <iostream>

using namespace unitest;

int main() {
    Runner tests {};

    tests.add({
        "add",
        "adds two numbers",
        [](const Test& assert) {
            const int x { 10 };
            const int y { 8 };
            
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
            const int x { 10 };
            const int y { 8 };

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
            // These extra initial asserts are used to show that the assertion count per-test is being incremented.
            assert.is_true(true);
            assert.is_false(false);

            assert.throws_no_exception<std::runtime_error>([]() { tests::mod(10, 0); });
        }
    });

    tests.add(tests::get_entity_tests());

    tests.go();

    return 0;
}
