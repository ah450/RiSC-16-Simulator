#pragma once
#include "asm/internal/sym.hpp"
#include <functional>
#include <memory>

namespace ass { namespace internal {

struct SymReference {
    std::shared_ptr<Sym> ref;
    SymReference(std::shared_ptr<Sym> &&ref) : ref(ref) {}
    bool operator==(const SymReference &other) const {
        return *ref == *(other.ref);
    }
    bool operator<(const SymReference & other) {
        return *ref  < *(other.ref);
    }
    
};
namespace std {
    // std::hash overload for ass::internal::SymReference
    template<>
    struct hash<ass::internal::SymReference S> {
    public:
        std::size_t operator()(S const& s) const {
            return std::hash<std::hash<std::string>()(s.ref->name);
        }
    };
} 

}}