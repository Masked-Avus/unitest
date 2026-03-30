# UniTest

## Description

UniTest is a simple, header-only library for unit testing C++ code. This library is based on the idea that doing something as straightforward as setting up unit tests should be as hassle-free as possible. The developer should not need to be weighed down by heavy dependencies to just to accomplish that task. To meet that desire, UniTest consists of a single header file; just `#include "unitest.hpp"`, and you are good to go! Also, rather than relying on a bunch of macros, this library uses the power of templates to provide an explicit, type-safe mechanism for running unit tests.

UniTest is designed to be compatible with C++11 and later.

## Use
### Basics

To use UniTest, first fire up a `unitest::Runner` object.

```cpp
unitest::Runner tests {};
```

To add a test to the `Runner`, call the `add` function, which requires the name of your test subject, a string denoting what result you expect for a success, and a test function as parameters. As for the test function, you can pass either a raw function pointer or a lambda function. Whether using a raw function pointer or named function objects, a convention used by the author for naming test functions is to use a `PascalCase` name that contains both the thing being tested and the expected result, with the two separated by an underscore. 

```cpp
// You can pass in a raw function pointer...
tests.add({ "this_function", "passes", ThisFunction_Passes });
// ..or you can pass in a lambda function.
tests.add({ "this_other_function", "passes", [](const unitest::Test& assert) {
    assert.are_not_equal(0, 10);
}});
```

Should you run these tests, they will be placed under the `[UNGROUPED]` label within the output. That is because UniTest organizes all tests by `Group`. What we have done so far is add tests that are labelled as not belonging to a user-defined `Group`. To facilitate larger projects, we can define an explicit test `Group` and `add` it to the `Runner`.

```cpp
tests.add("My_Class", {
    { "method_1", "passes", Method1_Passes },
    { "method_2", "passes", Method2_Passes }
});
```

Once all your tests are loaded and ready to go, just call the `run` method, which returns the number of failed tests.

```cpp
const int failure_count { tests.run() };
std::cout << "Tests failed: " << failure_count << std::endl;
```

*Displaying the number of failed tests is not necessary, as `Runner` already does that after a pass.*

### Alternative Output

By default, `Runner` outputs to the console, but if you want to output to a different destination instead (like a file), you can pass a pointer to a `std::ostream` object into `Runner`'s constructor or its `set_output` method. *`Runner` does not take ownership of the `ostream`.*

```cpp
std::ostream file { "output.txt" };

unitest::Runner tests { &file };
```

```cpp
unitest::Runner tests {};

std::ostream file { "output.txt" };

tests.set_output(&file);
```

### Assertions

As of the current version, UniTest provides the following assertions:

| Assertion                     | Tests For                             |
|-------------------------------|---------------------------------------|
| `Test::are_equal`             | equality (`==`)                       |
| `Test::are_not_equal`         | inequality (`!=`)                     |
| `Test::is_greater`            | greater than (`>`)                    |
| `Test::is_greater_or_equal`   | greater than or equal (`>=`)          |
| `Test::is_less`               | less than (`<`)                       |
| `Test::is_less_or_equal`      | less than or equal (`<=`)             |
| `Test::is_true`               | `true` value                          |
| `Test:is_false`               | `false` value                         |
| `Test::is_nullptr`            | `nullptr` value                       |
| `Test::is_not_nullptr`        | not `nullptr` value                   |
| `Test::throws_exception`      | specific exception type thrown        |
| `Test::throws_any_exception`  | any exception type thrown             |
| `Test::throws_no_exception`   | specific exception type not thrown    |
| `Test::throws_no_exceptions`  | no exceptions thrown                  |

### Quirks

When using the assertions `are_equal`, `are_not_equal`, `is_greater`, `is_greater_or_equal`, `is_less`, or `is_less_or_equal` for testing operator overloads that target user-defined types, said functions will only compile if their respective operator overloads are free-floating functions as opposed to member functions. This is due to issues with the `this` pointer.

```cpp
class Will_Not_Work {
public:
    Will_Not_Work(int value) : m_value { value } { }

    // This will not work with UniTest.
    bool operator ==(const Will_Not_Work& other) {
        return m_value == other.m_value;
    }

private:
    int m_value {};
};

class Will_Work {
public:
    Will_Work(int value) : m_value { value } { }

    friend bool operator ==(const Will_Work&, const Will_Work&);

private:
    int m_value {};
};

// This will work with UniTest.
bool operator ==(const Will_Work& left, const Will_Work& right) {
    return left.m_value == right.m_value;
}
```

## Example

```cpp
#include "unitest.hpp"

int this_function() { return 10; }
int this_other_function(int x, int y, int z) { return (x * y) / z; }

class This_Type {
public:
    This_Type(int value) : m_value { value } { }

    int get_value() const { return value; }
    int set_value(int value) { m_value = value; }

private:
    int m_value {};
};

void ThisFunction_Passes(const unitest::Test& assert) {
    assert.are_equal(this_function, 10);
}

int main() {
    unitest::Runner tests {};

    // Add ungrouped tests.
    tests.add("this_function", "returns expected number", ThisFunction_Passes);
    tests.add("this_other_function", "returns expected calculation", [](const unitest::Test& assert) {
        assert.are_equal(this_other_function(2, 10, 2), 10);
    });

    // Add grouped tests.
    tests.add({
        "My_Type",
        {
            "get_value", "returns expected value", [](const unitest::test& assert) {
                constexpr int expected { 8 };
                My_type instance { expected };

                assert.are_equal(expected, instance.get_value());
            },
            "set_value", "sets value to expected", [](const unitest::test& assert) {
                My_type instance { 5 };

                constexpr int expected { 8 };
                instance.set_value(expected(expected));
                
                assert.are_equal(expected, instance.get_value());
            }
        }
    });

    return tests.run();
}
```

## Building the Unit Tests

To create and run this project's unit tests, you will need to have Premake installed. If you do, use it to run the premake5.lua file with your toolset of choice and run the generated build files. Below is an example using Makefiles.

```
premake5 gmake
```

Alternatively, if you have PowerShell and Make installed, you can use the `build.ps1` file to build the test projects and use the `run.ps1` file to run them.

## Installation

UniTest consists of just a single source file. All you have to do is copy the `unitest.hpp` file within `src/` into your project and `#include` it.

