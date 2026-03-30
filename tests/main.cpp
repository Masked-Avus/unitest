#include "unitest.hpp"
#include <string>
#include <stdexcept>
#include <iostream>
#if UNITEST_TESTS_FILE_OUTPUT
    #include <fstream>
#endif

using namespace unitest;

namespace tests {

class Entity final {
public:
    Entity(const std::string& name, int id) :
        m_name { name },
        m_id { id } { }

    const std::string& get_name() const { return m_name; }
    const int& get_id() const { return m_id; }

    bool operator ==(const Entity& other) { return get_id() == other.get_id(); }
    bool operator !=(const Entity& other) { return get_id() != other.get_id(); }

    int get_hash() const { return m_id ^ sizeof(int); }

private:
    std::string m_name {};
    int m_id {};
};

unitest::Group get_entity_tests() {
    return unitest::Group("Entity")
    .add({
        "Entity::get_hash()",
        "returns expected hash value",
        [](const unitest::Test& assert) {
            const int value { 100 };
            const tests::Entity entity { "something", value };
            
            assert.are_equal(entity.get_hash(), static_cast<int>(value ^ sizeof(int)));
        }
    })
    .add({
        "Entity::get_name()",
        "has expected name",
        [](const unitest::Test& assert) {
            const std::string name { "Bob" };
            const tests::Entity entity { name, 8 };

            assert.are_equal(name, entity.get_name());
        }
    })
    .add({
        "Entity::get_id()",
        "has expected ID",
        [](const unitest::Test& assert) {
            const int id { 1000 };
            const tests::Entity entity { "another", id };

            assert.are_equal(id, entity.get_id());
        }
    });
}

int add(int x, int y) {
    return x + y;
}

int sub(int x, int y) {
    return x + y;
}

int mul(int x, int y) {
    return x * y;
}

int div(int x, int y) {
    if (y == 0) {
        throw std::runtime_error("Cannot divide by 0");
    }

    return x / y;
}

int mod(int x, int y) {
    if (y == 0) {
        throw std::runtime_error("Cannot divide by 0");
    }

    return x % y;
}

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

}

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

    // This is supposed to fail.
    tests.add({
        "boolean test",
        "fails",
        [](const Test& assert) {
            assert.is_true(false);
        }
    });

    tests.add(tests::get_entity_tests());

    // This is supposed to fail.
    tests.add({
        "throw assertion",
        "reports unexpected exception type upon throw",
        [](const Test& assert) {
            assert.throws_exception<std::runtime_error>([]() {
                tests::throw_custom_base_exception();
            });
        }
    });

    tests.add({
        "any exception test",
        "succeeds with any assertion that is thrown",
        [](const Test& assert) {
            assert.throws_any_exception([](){
                tests::throw_custom_base_exception();
            });
        }
    });

    // This is supposed to fail.
    tests.add({
        "no exception test",
        "fails with no assertions thrown",
        [](const Test& assert) {
            assert.throws_no_exception([]() {
                tests::throw_custom_base_exception();
            });
        }
    });

    // This is supposed to fail.
    tests.add({ "equality test", "fails", [](const Test& assert) {
        assert.are_equal(8, 10);
    }});
    // This is supposed to fail.
    tests.add({ "inequality test", "fails", [](const Test& assert) {
        assert.are_not_equal(8.8, 8.8);
    }});
    tests.add({ "greater-than test", "passes", [](const Test& assert) {
        assert.is_greater(10, 5);
    }});
    tests.add({ "greater-than test", "fails", [](const Test& assert) {
        assert.is_greater(5, 10);
    }});

    tests.run();

    return 0;
}
