#include "asm/assembler.hpp"
#include "asm/internal/regex.hpp"
#include "asm/internal/instructions.hpp"
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <utility>
#include <string>
#include <memory>

namespace ass { namespace internal {


namespace bfs = boost::filesystem;

typedef std::uint16_t addr_t;
typedef std::uint16_t pc_t;
typedef std::vector<std::pair<std::unique_ptr<bfs::ifstream>, bfs::path>> ifvector_t;
enum class SymType {INST};

struct Sym {
    bool defined;
    addr_t address;
    SymType type;
};



struct AssemblingStatus {
    bool error, originDefined;  
    pc_t pc;
    addr_t origin;
    std::unordered_map<std::string, Sym> symbols;
    ILogger * logger;
    AssemblingStatus(ILogger * logger) : error(false), originDefined(false), pc(0), symbols(), logger(logger){} 


};



void getOrigin(const ifvector_t &files, AssemblingStatus &state);


}}
