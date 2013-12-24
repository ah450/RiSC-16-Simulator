#pragma once
#include "asm/internal/sym.hpp"
#include <functional>
#include <cstdint>
#include <memory>
#include <string>


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
}}

namespace std {
    // std::hash overload for ass::internal::SymReference
   template <> struct hash<ass::internal::SymReference> {
    size_t operator()(const ass::internal::SymReference & ref) const {
      return hash<std::string>()(ref.ref->name());
    }
  };
} 