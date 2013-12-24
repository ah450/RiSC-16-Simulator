#pragma once
#include "asm/internal/sym_reference.hpp"
#include <boost/filesystem.hpp>
#include <string>
#include <map>
#include <set>

namespace ass {internal {
namespace bfs = boost::filesystem;

struct FileState {
    std::set<SymReference> importedSyms;
    std::set<SymReference> exportedSyms;
    std::set<SymReference> localSyms;
    std::multimap<SymReference, pc_t> localDeps;
    bfs::path name;
    FileState(const std::string name);
};

}}