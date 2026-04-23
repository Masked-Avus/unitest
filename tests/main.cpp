#include "unitest.hpp"

#include "entity.hpp"
#include "simple_math.hpp"
#include "custom_exceptions.hpp"
#include "string_view_tests.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#if UNITEST_TESTS_FILE_OUTPUT
    #include <fstream>
#endif

using namespace unitest;

namespace tests = unitest::tests;

void TestWithFunctionPointer_Passes(const Test& assert) {
    assert.is_true(true);
    assert.is_false(false);
}

int main() {
#if !UNITEST_TESTS_FILE_OUTPUT
    print("Starting tests...");
    print(1);
    print(" ", 2);
    print_line(" ", 3);
    print_line("Go!");
    print_line();
#endif

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
    tests.add({ "TestWithFunctionPointer_Passes", "passes", TestWithFunctionPointer_Passes });

    tests.add({ "add", "adds two numbers", [](const Test& assert) {
        constexpr int x { 10 };
        constexpr int y { 8 };
        
        const int expected { x + y };
        const int actual { tests::add(x, y) };

        assert.are_equal(expected, actual);
        assert.is_true(expected == actual);
    }});

    tests.add({ "sub", "subtracts two numbers", [](const Test& assert) {
        constexpr int x { 10 };
        constexpr int y { 8 };

        const int expected { (x - y) + 1 };
        const int actual { tests::sub(x, y) };

        assert.are_not_equal(expected, actual);
        assert.is_false(expected == actual);
    }});

    tests.add({ "mul", "multiplies one number by another", [](const Test& assert) {
        assert.throws_no_exception<std::runtime_error>([]() { tests::mul(10, 5); });
    }});

    tests.add({ "div", "throws exception if attempting to divide by zero", [](const Test& assert) {
        assert.throws_exception<std::runtime_error>([]() { tests::div(10, 0); });
    }});

    // This is supposed to fail.
    tests.add({ "mod", "does not throw exception if attempting to divide by zero", [](const Test& assert) {
        // These extra initial asserts are used to show that the assertion count per-test is
        //     being incremented.
        assert.is_true(true);
        assert.is_false(false);

        assert.throws_no_exception<std::runtime_error>([]() { tests::mod(10, 0); });
    }});

    // This is supposed to fail.
    // Testing custom assertion failure messages.
    tests.add({ "div", "throws an exception if attempting to divide by zero", [](const Test& assert) {
        assert.throws_exception<std::runtime_error>([]() { tests::mod(10, 5); }, "LOL, assertion failure");
    }});

    // This is supposed to fail.
    tests.add({ "boolean test", "fails", [](const Test& assert) {
        assert.is_true(false);
    }});

    // This is supposed to fail.
    tests.add({ "throw assertion", "reports unexpected exception type upon throw", [](const Test& assert) {
        assert.throws_exception<std::runtime_error>([]() { tests::throw_custom_base_exception(); });
    }});

    tests.add({ "throw assertion", "captures exception of base class", [](const Test& assert) {
        assert.throws_exception<tests::Custom_Base_Exception>([]() { tests::throw_custom_derived_exception(); });
    }});

    tests.add({ "any-exception test", "succeeds with any assertion that is thrown", [](const Test& assert) {
        assert.throws_any_exception([](){ tests::throw_custom_base_exception(); });
    }});

    // This is supposed to fail.
    tests.add({ "no exception test", "fails with no assertions thrown", [](const Test& assert) {
        assert.throws_no_exceptions([]() { tests::throw_custom_base_exception(); });
    }});

// Use of macros instead of constexpr variables to keep lambda declarations short.
#define GREATER_NUMBER 10
#define LESSER_NUMBER 5

    // This is supposed to fail.
    tests.add({ "equality-test", "fails", [](const Test& assert) {
        assert.are_equal(LESSER_NUMBER, GREATER_NUMBER);
    }});

    // This is supposed to fail.
    tests.add({ "inequality-test", "fails", [](const Test& assert) {
        assert.are_not_equal(8.8, 8.8);
    }});

    tests.add({ "greater-than test", "passes", [](const Test& assert) {
        assert.is_greater(GREATER_NUMBER, LESSER_NUMBER);
    }});

    // This is supposed to fail.
    tests.add({ "greater-than test", "fails", [](const Test& assert) {
        assert.is_greater(LESSER_NUMBER, GREATER_NUMBER);
    }});

    tests.add({ "greater-than-or-equal test", "passes", [](const Test& assert) {
        assert.is_greater_or_equal(GREATER_NUMBER, GREATER_NUMBER);
        assert.is_greater_or_equal(GREATER_NUMBER, LESSER_NUMBER);
    }});

    // This is supposed to fail.
    tests.add({ "greater-than-or-equal test", "fails", [](const Test& assert) {
        assert.is_greater_or_equal(LESSER_NUMBER, GREATER_NUMBER);
    }});

    tests.add({ "less-than test", "passes", [](const Test& assert) {
        assert.is_less(LESSER_NUMBER, GREATER_NUMBER);
    }});

    // This is supposed to fail.
    tests.add({ "less-than test", "fails", [](const Test& assert) {
        assert.is_less(GREATER_NUMBER, LESSER_NUMBER);
    }});

    tests.add({ "less-than-or-equal test", "passes", [](const Test& assert){
        assert.is_less_or_equal(LESSER_NUMBER, GREATER_NUMBER);
        assert.is_less_or_equal(LESSER_NUMBER, LESSER_NUMBER);
    }});

    // This is supposed to fail.
    tests.add({ "less-than-or-equal test", "fails", [](const Test& assert){
        assert.is_less_or_equal(GREATER_NUMBER, LESSER_NUMBER);
    }});

    // This is supposed to fail.
    tests.add({ "char-comparison", "fails", [](const Test& assert) {
        assert.are_equal('c', 'a');
    }});

#undef GREATER_NUMBER
#undef LESSER_NUMBER

    tests.add(tests::get_entity_tests());
    tests.add(tests::get_string_view_tests());

    // This should not be printed.
    tests.add({ "Empty" });

    tests.run();

    return 0;
}
