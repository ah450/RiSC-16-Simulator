#pragma once
#include "asm/internal/typedefs.hpp"
#include "asm/internal/instruction.hpp"
#include "asm/internal/sym_reference.hpp"
#include "asm/internal/file_state.hpp"
#include <boost/filesystem.hpp>
#include <unordered_map>
#include <memory>
#include <list>
#include <set>
#include <map>

namespace std {
template<>
struct hash<boost::filesystem::path> {
    size_t operator()( const boost::filesystem::path &p ) const {
        return hash<std::string>()( p.generic_string() );
    }
};
}
namespace ass {
namespace internal {

namespace bfs = boost::filesystem;

class AssemblingStatus {
    bool error, originDefined, mainDefined;
    addr_t origin_, main_;
    std::shared_ptr<ILogger> logger;
    std::unordered_map<bfs::path, FileState> files;
    std::list<Instruction> insts;
    std::set<SymReference> allReferences;
    std::multimap<SymReference, pc_t> dependencies;
public:
    AssemblingStatus( std::shared_ptr<ILogger> logger );
    FileState &getState( const bfs::path &path ) {
        if ( files.count( path ) == 0 ) {
            files.emplace( std::make_pair( path, FileState( path ) ) );
        }

        return files.at( path );
    }
    void defineOrigin( const addr_t &addr ) {
        if ( !originDefined ) {
            originDefined = true;
            origin_ = addr;
        } else {
            // defined multiple times
            error = true;
        }
    }
    void defineMain( addr_t addr ) {
        if ( !mainDefined ) {
            mainDefined = true;
            main_ = addr;

        } else {
            *logger << "Main redefined signaling error\n";
            error = true;
        }
    }
    addr_t origin() {
        return origin_;
    }
    addr_t main() {
        return main_;
    }
    bool isOriginDefined() {
        return originDefined;
    }
    bool isMainDefined() {
        return mainDefined;
    }
    template<typename T>
    AssemblingStatus &operator<<( T&& t ) {
        *logger << t;
        return *this;
    }

    void signalError() {
        error = true;
    }

    std::multimap<SymReference, pc_t> &deps() {
        return dependencies;
    };
    std::set<SymReference> &syms() {
        return allReferences;
    }
    std::list<Instruction> &instList() {
        return insts;
    }
    bool ok() {
        return !error;
    }
};

}
}