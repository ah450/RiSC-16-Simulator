#include "asm/internal/internal.hpp"

using namespace ass::internal;



FileState::FileState(const std::string name) : importedSyms(), exportedSyms(), name(name){

}


bool FileState::isImported(const SymReference &sym) const {
    return importedList.find(sym) == std::end(importedSyms);
}

bool FileState::isExported(const SymReference &sym) const {
    return exportedSyms.find(sym) == std::end(importedSyms);
}