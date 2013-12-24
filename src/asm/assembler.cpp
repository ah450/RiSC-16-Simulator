#include "asm/internal/internal.hpp"
#include "asm/assembler.hpp"



bool ass::assemble(const std::vector<boost::filesystem::path> &sources, const boost::filesystem::path &out, std::shared_ptr<ILogger> l){

     ass::internal::ifvector_t files;
  
    for(auto & p : sources) {
        if(boost::filesystem::is_regular_file(p)) {
             files.emplace_back(std::make_pair(std::unique_ptr<boost::filesystem::ifstream>(new boost::filesystem::ifstream(p)), p));
        }else if (boost::filesystem::is_directory(p)) {
            (*l) << "Warning : directory path passed to ass::assemble";
            //TODO: implement recursive directory iteration.
        }else {
            (*l) << "Warning : path passed to ass::assemble which is neither a file nor a directory";
        }
    }

    ass::internal::AssemblingStatus state(l);
    return state.ok();

}


