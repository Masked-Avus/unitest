#pragma once

#include <string>
#include "unitest.hpp"

namespace unitest {
namespace tests {

class Entity final {
public:
    Entity(const std::string& name, int id) :
        m_name { name },
        m_id { id } { }

    const std::string& get_name() const { return m_name; }
    const int& get_id() const { return m_id; }

    int get_hash() const { return m_id ^ sizeof(int); }

private:
    std::string m_name {};
    int m_id {};
};

inline bool operator ==(const Entity& left, const Entity& right) { return left.get_id() == right.get_id(); }
inline bool operator !=(const Entity& left, const Entity& right) { return left.get_id() != right.get_id(); }

Group get_entity_tests();

}
}
