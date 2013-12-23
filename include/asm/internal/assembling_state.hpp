#pragma once
#include "asm/internal/typedefs.hpp"
#include "asm/internal/instruction.hpp"
#include "asm/internal/sym_reference.hpp"
#include "asm/internal/file_state.hpp"
#include <boost/filesystem.hpp>
#include <memory>
#include <list>
#include <set>
#include <map>

namespace ass { namespace internal {

namespace bfs = boost::filesystem;
class AssemblingStatus {
    bool error, originDefined;  
    addr_t origin;
    std::shared_ptr<ILogger> logger;
    std::unordered_map<bfs::path, FileState> files;
    std::list<Instruction> insts;
    std::set<SymReference> allReferences;
    std::multimap<SymReference, pc_t> dependencies;
public:
    AssemblingStatus(std::shared_ptr<ILogger> logger);
    std::FileState & getState(const bfs::path & path) {
        if(files.count(path) == 0){
            files.emplace(std::make_pair(path, FileState(path)));
        }
        return files[path];
    }
    void defineOrigin(const addr_t &addr) {
        if(!originDefined){
            originDefined = true;
            origin = addr;
        }else {
            // defined multiple times
            error = true;
        }
    }

    template<typename T>
    AssemblingStatus & operator<<(T&& t) {*logger << t; return *this;}

    void signalError() { error = true;}
    std::multi_map<SymReference, std::size_t> & deps() { return dependencies;};
    std::set<SymReference> & syms() { return allReferences;}
    std::list<instructions> insts & instList() {return insts};

};

}}