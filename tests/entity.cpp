#include "entity.hpp"

namespace unitest {
namespace tests {

Group get_entity_tests() {
    return Group("Entity")
    .add({ "Entity::get_hash()", "returns expected hash value", [](const Test& assert) {
        constexpr int value { 100 };
        const Entity entity { "something", value };
        
        assert.are_equal(entity.get_hash(), static_cast<int>(value ^ sizeof(int)));
    }})
    .add({ "Entity::get_name()", "has expected name", [](const Test& assert) {
        const std::string name { "Bob" };
        const Entity entity { name, 8 };

        assert.are_equal(name, entity.get_name());
    }})
    .add({ "Entity::get_id()", "has expected ID", [](const Test& assert) {
        constexpr int id { 1000 };
        const Entity entity { "another", id };

        assert.are_equal(id, entity.get_id());
    }})
    .add({ "Entity::operator==", "returns true for equal entities", [](const Test& assert) {
        constexpr int id { 1000 };
        const Entity first { "first", id };
        const Entity second { "second", id };

        assert.are_equal(first, second);
    }});
}
    
}
}
