#pragma once

#include <string>
#include <stdexcept>
#include "unitest.hpp"

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

inline unitest::Group get_entity_tests() {
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

inline int add(int x, int y) {
    return x + y;
}

inline int sub(int x, int y) {
    return x + y;
}

inline int mul(int x, int y) {
    return x * y;
}

inline int div(int x, int y) {
    if (y == 0) {
        throw std::runtime_error("Cannot divide by 0");
    }

    return x / y;
}

inline int mod(int x, int y) {
    if (y == 0) {
        throw std::runtime_error("Cannot divide by 0");
    }

    return x % y;
}

}
