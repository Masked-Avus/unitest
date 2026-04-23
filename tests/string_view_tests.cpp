#include "unitest.hpp"

namespace unitest {
namespace tests {

const char s_godzilla[] { "Godzilla" };
const char s_ghidorah[] { "King Ghidorah" };

Group get_string_view_tests() {
    return Group("String_View")
    .add({ "String_View::String_View", "creates view of const char*", [](const Test& assert) {
        const String_View view { s_godzilla };

        assert.are_equal(view.get_length(), std::strlen(s_godzilla));

        for (int i {}; i < view.get_length(); ++i) {
            const char expected { s_godzilla[i] };
            const char actual { view[i] };
            assert.are_equal(expected, actual);
        }
    }})
    .add({ "String_View::String_View", "creates view of std::string", [](const Test& assert) {
        const std::string str { s_godzilla };
        const String_View view { str };

        assert.are_equal(view.get_length(), str.length());

        for (int i {}; i < view.get_length(); ++i) {
            const char expected { str[i] };
            const char actual { view[i] };
            assert.are_equal(expected, actual);
        }
    }})
    .add({ "String_View::to_string", "creates new std::string from contents of view", [](const Test& assert) {
        const std::string str { s_godzilla };
        const String_View view { str };
        const std::string copy { view.to_string() };

        assert.are_equal(str.length(), copy.length()); 

        for (int i {}; i < str.length(); ++i) {
            const char expected { str[i] };
            const char actual { copy[i] };
            assert.are_equal(expected, actual);
        }
    }});
}

}
}
