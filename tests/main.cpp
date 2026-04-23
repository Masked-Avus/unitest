#include "unitest.hpp"

#include <string>
#include <stdexcept>
#include <iostream>
#if UNITEST_TESTS_FILE_OUTPUT
    #include <fstream>
#endif

namespace unitest {
namespace tests {

void load_ungrouped_tests(Runner& tests);
Group get_entity_tests();
Group get_string_view_tests();

}
}

using namespace unitest;

namespace tests = unitest::tests;

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
    
    tests::load_ungrouped_tests(tests);
    tests.add(tests::get_entity_tests());
    tests.add(tests::get_string_view_tests());

    // This should not be printed.
    tests.add({ "Empty" });

    tests.run();

    return 0;
}
