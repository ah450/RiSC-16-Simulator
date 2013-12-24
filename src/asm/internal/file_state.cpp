#include "asm/internal/file_state.hpp"

ass::internal::FileState::FileState(const bfs::path &name) : importedSyms(),
exportedSyms(), localSyms(), localDeps(), name(name) {

}


