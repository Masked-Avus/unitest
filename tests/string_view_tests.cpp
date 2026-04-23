#include "string_view_tests.hpp"

namespace unitest {
namespace tests {

const char s_godzilla[] { "Godzilla" };
const char s_ghidorah[] { "King Ghidorah" };

Group get_string_view_tests() {
    return Group("String_View")
    .add({ "String_View::String_View", "creates view of const char*", [](const Test& assert) {
        String_View view { s_godzilla };

        assert.are_equal(view.get_length(), std::strlen(s_godzilla));

        for (int i {}; i < view.get_length(); ++i) {
            const char expected { s_godzilla[i] };
            const char actual { view[i] };
            assert.are_equal(expected, actual);
        }
    }});
}

}
}
