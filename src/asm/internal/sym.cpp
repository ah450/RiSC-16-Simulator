#include "asm/internal/sym.hpp"


ass::internal::Sym(bool global, const std::string &name,
        bfs::path file, std::size_t lineNum,  addr_t value, SymType type) : defined_(true),
        global_(global), address_(value), type_(type), name_(name), file_(file), lineNum_(lineNum) {
            
}

