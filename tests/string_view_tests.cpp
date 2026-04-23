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
    }})
    .add({"String_View equality operations", "return true if equal and false if not", [](const Test& assert) {
        const String_View first { s_godzilla };
        const String_View second { s_godzilla };
        const String_View third { s_ghidorah };

        assert.is_true(first.equals(second));
        assert.is_true(first == second);

        assert.is_true(second.equals(first));
        assert.is_true(second == first);

        assert.is_false(first.equals(third));
        assert.is_false(first == third);

        assert.is_false(second.equals(third));
        assert.is_false(second == third);

        assert.is_false(third.equals(first));
        assert.is_false(third == first);

        assert.is_false(third.equals(second));
        assert.is_false(third == second);

        assert.is_true(first != third);
        assert.is_true(second != third);
        assert.is_true(third != first);
        assert.is_true(third != second);
    }})
    .add({ "String_View comparison operations", "return expected sort order values", [](const Test& assert) {
        const String_View first { s_godzilla };
        const String_View second { s_godzilla };
        const String_View third { s_ghidorah };

        assert.is_true(first.compare(second) == String_View::Sort_Order::Same);
        assert.is_true(second.compare(first) == String_View::Sort_Order::Same);

        assert.is_true(first.compare(third) == String_View::Sort_Order::Before);
        assert.is_true(second.compare(third) == String_View::Sort_Order::Before);
        assert.is_true(first < third);
        assert.is_true(second < third);

        assert.is_true(third.compare(first) == String_View::Sort_Order::After);
        assert.is_true(third.compare(second) == String_View::Sort_Order::After);
        assert.is_true(third > first);
        assert.is_true(third > second);
    }});
}

}
}
