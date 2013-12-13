#include "asm/assembler.hpp"
#include <boost/filesystem/fstream.hpp>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <string>
#include <memory>


namespace bfs = boost::filesystem;

typedef std::uint16_t addr_t;
typedef std::uint16_t pc_t;
enum class SymType {INST};

struct Sym {
    bool defined;
    addr_t address;
    SymType type;
};

struct AssemblingStatus {
    bool error;   
    pc_t pc;
    std::unordered_map<std::string, Sym> symbols;
    ILogger * logger;
    AssemblingStatus(ILogger * logger) : error(false), pc(0), symbols(), logger(logger){} 


};

bool ass::assemble(const std::vector<bfs::path> &sources, const bfs::path &out, ILogger *l){

     std::vector<std::unique_ptr<bfs::ifstream>> files;

    for(const bfs::path& p : sources) {
        if(bfs::is_regular_file(p)) {
             files.emplace_back(new bfs::ifstream(p));
        }else if (bfs::is_directory(p)) {
            (*l) << "Warning : directory path passed to ass::assemble";
            //TODO: implement recursive directory iteration.
        }else {
            (*l) << "Warning : path passed to ass::assemble which is neither a file nor directory";
        }
    }

    AssembStatus state;

    return !state.error;

}