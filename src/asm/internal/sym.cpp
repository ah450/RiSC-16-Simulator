#include "asm/internal/internal.hpp"

using namespace ass::internal;

Sym::Sym(bool defined, bool global, addr_t value, SymType type, std::string name,
        bfs::path file, std::size_t lineNum) : defined(defined), global(global), 
                        address(value), type(type), name(name), file(file), lineNum(lineNum){
                            
}
