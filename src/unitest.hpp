// Version: 1.0.1

#pragma once

#include <string>
#include <cstring>
#include <vector>
#include <exception>
#include <ostream>
#include <iostream>
#include <functional>
#include <chrono>

namespace unitest {

class Test;

/*
We provide this to allow for compatibility with C++11. For the sake of simplicity, unlike
    std::string_view, this take on a string view does not allow any way of manipulating the view's
    size.
*/
class String_View final {
public:
    String_View() { }
    String_View(const char* source) :
        m_data { source },
        m_length { std::strlen(source) } { }
    String_View(const char* source, std::size_t length) :
        m_data { source },
        m_length { length } { }
    String_View(const std::string& source) :
        m_data { source.data() },
        m_length { source.length() } { }
    
    std::size_t get_length() const { return m_length; }
    bool is_empty() const { return m_length == 0; }

    std::string to_string() const {
        if (is_empty()) {
            return "";
        }

        std::string result {};

        for (std::size_t i; i < m_length; ++i) {
            result.push_back(*(m_data + i));
        }

        return result;
    }

    char operator [](std::size_t index) { return *(m_data + index); }
    char operator [](std::size_t index) const { return *(m_data + index); }
    operator const char*() const { return m_data; }

    friend std::ostream& operator <<(std::ostream&, String_View);

private:
    const char* m_data {};
    std::size_t m_length {};
};

inline std::ostream& operator <<(std::ostream& output, String_View string) {
    for (std::size_t i {}; i < string.m_length; ++i) {
        output << string[i];
    }

    return output;
}

enum class Status {
    Pending = -1,
    Success = 0,
    Failure = 1
};

inline const char* to_string(Status status) {
    switch (status) {
    case Status::Success:
        return "SUCCESS";

    case Status::Failure:
        return "FAILURE";

    case Status::Pending:
        return "PENDING";

    default:
        throw std::runtime_error("Unknown status value");
    }
}

class Assertion_Failure final : public std::exception {
public:
    Assertion_Failure(const Test& associated_test, const std::string& message, int assertion_number);
    Assertion_Failure(const Test& associated_test, std::string&& message, int assertion_number);

    const std::string& get_subject() const { return m_test_subject; }
    const std::string& get_expected_result() const { return m_test_expected_result; }
    String_View get_failure_message() const { return m_failure_message; }
    int get_assertion_number() const { return m_assertion_number; }

private:
    std::string m_test_subject {};
    std::string m_test_expected_result {};
    std::string m_failure_message {};
    int m_assertion_number {};
};

class Test final {
public:
    Test(String_View subject, String_View expected_result, std::function<void(const Test&)> function) :
        m_subject { subject },
        m_expected_result { expected_result },
        m_function { function } { }

    const std::string& get_name() const { return m_subject; }
    const std::string& get_expected_result() const { return m_expected_result; }
    bool has_passed() const { return m_status == Status::Success; }
    bool has_failed() const { return m_status == Status::Failure; }
    Status get_status() const { return m_status; }

public: // Assertions
    // Assertions are called by the test object passed into a user-defined test function.

    template<typename T>
    void are_equal(const T& expected, const T& actual) const {
        ++m_assertion_count;
        internal_assert(expected == actual, "equality");
    }
    
    template<typename T>
    void are_equal(const T& expected, const T& actual, String_View message) const {
        ++m_assertion_count;
        internal_assert_with_custom_message(expected == actual, message);
    }
    
    template<typename T>
    void are_not_equal(const T& expected, const T& actual) const {
        ++m_assertion_count;
        internal_assert(expected != actual, "inequality");
    }
    
    template<typename T>
    void are_not_equal(const T& expected, const T& actual, String_View message) const {
        ++m_assertion_count;
        internal_assert_with_custom_message(expected != actual, message);
    }
    
    void is_true(bool expression) const {
        ++m_assertion_count;
        internal_assert(expression, "true", "false");
    }
    
    void is_true(bool expression, String_View message) const {
        ++m_assertion_count;
        internal_assert_with_custom_message(expression, message);
    }

    void is_false(bool expression) const {
        ++m_assertion_count;
        internal_assert(!expression, "false", "true");
    }

    void is_false(bool expression, String_View message) const {
        ++m_assertion_count;
        internal_assert_with_custom_message(!expression, message);
    }

    template<typename T>
    void is_nullptr(const T* ptr) const {
        ++m_assertion_count;
        internal_assert(ptr == nullptr, "nullptr");
    }
    
    template<typename T>
    void is_nullptr(const T* ptr, String_View message) const {
        ++m_assertion_count;
        internal_assert_with_custom_message(ptr == nullptr, message);
    }
    
    template<typename T>
    void is_not_nullptr(const T* ptr) const {
        ++m_assertion_count;
        internal_assert(ptr != nullptr, "no nullptr");
    }
    
    template<typename T>
    void is_not_nullptr(const T* ptr, String_View message) const {
        ++m_assertion_count;
        internal_assert_with_custom_message(ptr != nullptr, message);
    }
    
    template<typename Exception_Type>
    void throws_exception(std::function<void()> function) const {
        ++m_assertion_count;

        try {
            function();
            internal_assert(false, "an exception to be thrown");
        }
        catch (Exception_Type& exception) {
        }
    }

    template<typename Exception_Type>
    void throws_exception(std::function<void()> function, String_View message) const {
        ++m_assertion_count;

        try {
            function();
            internal_assert_with_custom_message(false, message);
        }
        catch (Exception_Type& exception) {
        }
    }
    
    template<typename Exception_Type>
    void throws_no_exception(std::function<void()> function) const {
        ++m_assertion_count;

        try {
            function();
        }
        catch (Exception_Type exception) {
            internal_assert(false, "no exception to be thrown");
        }
    }
    
    template<typename Exception_Type>
    void throws_no_exception(std::function<void()> function, String_View message) const {
        ++m_assertion_count;

        try {
            function();
        }
        catch (Exception_Type exception) {
            internal_assert_with_custom_message(false, message);
        }
    }
    
private:
    std::function<void(const Test&)> m_function {};
    std::string m_subject {};
    std::string m_expected_result {};
    Status m_status { Status::Pending };
    // Stops at the first failed assertion.
    mutable int m_assertion_count {};
    
    void operator ()() {
        try {
            m_function(*this);
            flag_success();
        }
        catch (const Assertion_Failure& exception) {
            flag_failure();
            throw;
        }
        // Let other exceptions bubble up for debugging purposes.
    }

    void reset() {
        m_assertion_count = 0;
        m_status = Status::Pending;
    }
        
    void flag_success() { m_status = Status::Success; }
    void flag_failure() { m_status = Status::Failure; }

    void internal_assert(bool is_true, String_View expected) const {
        if (!is_true) {
            throw Assertion_Failure(
                *this,
                std::forward<std::string>(std::string().append("expected ").append(expected)),
                m_assertion_count
            );
        }
    }

    void internal_assert_with_custom_message(bool is_true, String_View message) const {
        if (!is_true) {
            throw Assertion_Failure(
                *this,
                std::forward<std::string>(std::string().append(message)),
                m_assertion_count
            );
        }
    }

    void internal_assert(bool is_true, String_View expected, String_View actual) const {
        if (!is_true) {
            throw Assertion_Failure(
                *this,
                std::forward<std::string>(std::string()
                    .append("expected ")
                    .append(expected)
                    .append(" but got ")
                    .append(actual)
                    .append(" instead\n")
                ),
                m_assertion_count
            );
        }
    }

    int get_assertion_count() const { return m_assertion_count; }

    friend class Runner;
    friend class Group;
    friend std::ostream& operator <<(std::ostream&, const Test&);
};

inline std::ostream& operator <<(std::ostream& output, const Test& test) {
    output << "Test("
        << test.m_subject << ',' << test.m_expected_result << ',' << to_string(test.m_status)
        << ')';
    return output;
}

inline Assertion_Failure::Assertion_Failure(
    const Test& associated_test,
    const std::string& message,
    int assertion_number
    ) :
    m_test_subject { associated_test.get_name() },
    m_test_expected_result { associated_test.get_expected_result() },
    m_failure_message { message },
    m_assertion_number { assertion_number } { }

inline Assertion_Failure::Assertion_Failure(
    const Test& associated_test,
    std::string&& message,
    int assertion_number
    ) :
    m_test_subject { associated_test.get_name() },
    m_test_expected_result { associated_test.get_expected_result() },
    m_failure_message { std::forward<std::string>(message) },
    m_assertion_number { assertion_number } { }

class Group {
public:
    Group(String_View name) :
        m_name { name } { }

    const std::string& get_name() const { return m_name; }
    std::size_t get_test_count() const { return m_tests.size(); }
    std::size_t get_success_count() const { return m_successes; }
    std::size_t get_failure_count() const { return m_failures.size(); }
    bool found_failures() const { return !m_failures.empty(); }
    const std::vector<Assertion_Failure> get_failure_records() const { return m_failures; }
    Test& operator [](std::size_t index) { return m_tests[index]; }
    const Test& operator [](std::size_t index) const { return m_tests[index]; }
    
    void reset() {
        for (Test& test : m_tests) {
            test.reset();
        }

        m_successes = 0;
        m_failures.clear();
    }

    Group& add(const Test& test) {
        m_tests.push_back(test);
        return *this;
    }

    Group& add(Test&& test) {
        m_tests.emplace_back(std::forward<Test>(test));
        return *this;
    }

    void run_all_tests() {
        reset();

        for (Test& test : m_tests) {
            try {
                test();
                ++m_successes;
            }
            catch (const Assertion_Failure& exception) {
                m_failures.emplace_back(exception);
            }
        }
    }

private:
    std::string m_name {};
    std::vector<Test> m_tests {};
    std::size_t m_successes {};
    std::vector<Assertion_Failure> m_failures {};
};

// This class does NOT take ownership of the output source.
class Runner final {
#define UNITEST_UNGROUPED_TESTS_NAME "[UNGROUPED]"
#define UNITEST_UNGROUPED_TESTS_LOCATION 0

public:
    Runner(std::ostream* output = &std::cout) :
        m_output { output }
        {
        
        m_tests.emplace_back(UNITEST_UNGROUPED_TESTS_NAME);
    }

    std::size_t get_success_count() const { return m_total_successes; }
    std::size_t get_failure_count() const { return m_total_failures; }
    std::size_t get_test_count() const { return m_test_count; }
    void set_output(std::ostream* output) { m_output = output; };

    void add(const Test& test) {
        m_tests[UNITEST_UNGROUPED_TESTS_LOCATION].add(test);
        ++m_test_count;
    }

    void add(Test&& test) {
        m_tests[UNITEST_UNGROUPED_TESTS_LOCATION].add(std::forward<Test>(test));
        ++m_test_count;
    }

    void add(const Group& group) {
        m_tests.emplace_back(group);
        m_test_count += group.get_test_count();
    }

    void add(Group&& group) {
        m_tests.emplace_back(std::forward<Group>(group));
        m_test_count += m_tests.back().get_test_count(); 
    }

    void run() {
        reset();
        output_current_date_time();

        for (std::size_t i {}; i < m_tests.size(); ++i) {
            Group test_group { m_tests[i] };
            (*m_output) << test_group.get_name() << ':' << '\n';
            test_group.run_all_tests();

            m_total_successes += test_group.get_success_count();
            m_total_failures += test_group.get_failure_count();

            output_results(test_group);
        }

        (*m_output)
            << m_total_successes << '/' << m_test_count << " tests succeeded and "
            << m_total_failures << '/' << m_test_count << " failed\n";
    }
    
private:
    std::vector<Group> m_tests {};
    std::ostream* m_output {};
    std::size_t m_total_failures {};
    std::size_t m_total_successes {};
    std::size_t m_test_count {};
    
    void reset() {
        for (Group& test_group : m_tests) {
            test_group.reset();
        }
        
        m_total_successes = 0;
        m_total_failures = 0;
    }

    void output_results(const Group& tests) {
        (*m_output) << "  Successes: "
            << tests.get_success_count() << '/' << tests.get_test_count() << '\n';

        if (tests.get_success_count() != tests.get_test_count()) {
            (*m_output) << "  Failures:  "
                << tests.get_failure_count() << '/' << tests.get_test_count() << '\n';

            for (const Assertion_Failure& failure : tests.get_failure_records()) {
                (*m_output)
                    << "    - " << failure.get_subject() << '\n'
                    << "      -> " << failure.get_expected_result() << '\n'
                    << "      -> assertion #" << failure.get_assertion_number() << ": "
                        << failure.get_failure_message() << '\n';
            }
        }
    }

    void output_current_date_time() {
        std::time_t time { std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) };
        (*m_output) << "Tests started on " << std::ctime(&time);
    }

#undef UNITEST_UNGROUPED_TESTS_NAME
#undef UNITEST_UNGROUPED_TESTS_LOCATION
};

}
