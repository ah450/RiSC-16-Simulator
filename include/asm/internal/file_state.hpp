#pragma once
#include "asm/internal/sym_reference.hpp"
#include <boost/filesystem.hpp>
#include <string>
#include <map>
#include <set>

namespace ass {
namespace internal {
namespace bfs = boost::filesystem;

struct FileState {
    std::set<SymReference> importedSyms;
    std::set<SymReference> exportedSyms;
    std::set<SymReference> localSyms;
    std::multimap<SymReference, pc_t> localDeps;
    bfs::path name;
    FileState( const bfs::path &name );
};

}
}