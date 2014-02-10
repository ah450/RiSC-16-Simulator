#pragma once
#include "core/typedefs.hpp"
#include <unordered_map>

namespace std {
template <>
struct hash<OP> {
    size_t operator()( OP op ) const {
        return std::hash<size_t>()( static_cast<size_t>( op ) );
    }
};
}


extern const std::unordered_map<OP, FUNCTION_UNIT> OP_MAPING;