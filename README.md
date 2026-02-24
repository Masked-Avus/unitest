# UniTest

## Description

UniTest is a simple header-only library for unit testing C++ code. This library is based on the idea that doing something as straightforward as setting up unit tests should be as hassle-free as possible, without being weighed down by heavy dependencies to accomplish that task. To meet that desire, UniTest consists of a single header file; just `#include "unitest.hpp"`, and you are good to go! Also, rather than relying on a bunch of macros, this library uses the power of templates to provide an explicit, type-safe mechanism for running unit tests.

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

By default, `Runner` outputs to the console, but if you want to output to a different destination instead (like a file), you can pass a pointer to a `std::ostream` object into `Runner`'s constructor. *`Runner` does not take ownership of the `ostream`.*

```cpp
std::ostream file { "output.txt" };

unitest::Runner tests { &file };
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
