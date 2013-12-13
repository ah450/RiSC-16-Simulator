#include "asm/internal/internal.hpp"

using namespace ass::internal;



bool ass::assemble(const std::vector<bfs::path> &sources, const bfs::path &out, ILogger *l){

     ifvector_t files;

    for(const bfs::path& p : sources) {
        if(bfs::is_regular_file(p)) {
             files.emplace_back(std::make_pair(std::unique_ptr<bfs::ifstream>(new bfs::ifstream(p)), p));
        }else if (bfs::is_directory(p)) {
            (*l) << "Warning : directory path passed to ass::assemble";
            //TODO: implement recursive directory iteration.
        }else {
            (*l) << "Warning : path passed to ass::assemble which is neither a file nor a directory";
        }
    }

    AssemblingStatus state(l);
    setOrigin(files, state);
    return !state.error;

}


