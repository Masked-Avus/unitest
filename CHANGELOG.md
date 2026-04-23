# Version History

## v1.4.0
- Added additional template specializations for asserting with `char`, `String_View` and `std::string` types.
- Added tests for `String_View` using UniTest itself.
- Added the following methods to `String_View`
    - `slice`
    - `remove_front`
    - `remove_back`
    - `equals`
    - `equals_caseless`
    - `compare`
    - `compare_caseless`
    - `begin`
    - `end`
    - `begin`
    - `cbegin`
- Added the following `friend` functions for `String_View`:
    - `operator ==`
    - `operator !=`
    - `operator >`
    - `operator <`
- Added `String_View_Const_Iterator` type for the new iterator functions possessed by `String_View`.
- Added `print` and `print_line` functions.

## v1.3.0
- Add the following new assertions to `Test`, with specializations for basic types:
    - `is_greater`
    - `is_greater_or_equal`
    - `is_less`
    - `is_less_or_equal`
- Rename `Test::throws_no_exception` to `Test::throws_no_exceptions`.

## v1.2.0
- Added template specializations of `Test::are_equal` and `Test::are_not_equal` for various atomic types (`int`, `long`, `float`, etc.) so their value can be printed when an assertion fails. That way, the user does not need to rely as heavily on tooling or impromptu print statements to figure out what a given value was when an assertion failed.

## v1.1.0
- Added author and license information to `unitest.hpp` (so the user knows what version of UniTest they have).
- Improved reporting to to `Test::throws_exception`.
- Added the following assertions to `Test`:
    - `throws_any_exception`
    - `throws_no_exception`

## v1.0.1
- Fixed assertion failure messages.

## v1.0.0
- Initial version.
