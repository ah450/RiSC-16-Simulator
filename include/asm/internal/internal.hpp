#include "asm/assembler.hpp"
#include "asm/internal/regex.hpp"
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <algorithm>
#include <iterator>
#include <cstdint>
#include <utility>
#include <string>
#include <memory>
#include <list>
#include <set>

namespace ass { namespace internal {


namespace bfs = boost::filesystem;

typedef std::uint16_t addr_t;
typedef std::uint16_t pc_t;
typedef std::pair<std::unique_ptr<bfs::ifstream>, bfs::path> file_path_pair_t;
typedef std::vector<file_path_pair_t> ifvector_t;
enum class SymType {PTR, DATA};
enum class InstType {LW, SW, ADDI, JMP, LI, JALR, JALI, ADD, SUB, MUL,
                    DIV, HALT, AND, OR, XOR, NOT, RET, BEQ, BNE, BGT, BGE, BLT, BLE};

class Sym : std::enable_shared_from_this<Sym>{
    bool defined;
    bool global;
    addr_t address;
    SymType type;
    std::string name;
    bfs::path file;
    std::size_t lineNum;
public:
    Sym(bool defined, bool global, addr_t value, SymType type, std::string name,
        bfs::path file, std::size_t lineNum);
    bool operator==(const Sym &other) const {
        return (file == other.file) && (name == other.name);
    }

};

class SymReference {
    std::shared_ptr<Sym> ref;
    bool operator==(const SymReference &other) const {
        return *ref == *(other.ref);
    }
};


struct Instruction {
    std::uint16_t pc;
    std::uint16_t data;
    InstType type;    
};

}} // end namespace ass internal

namespace std {
    // std::hash overload for ass::internal::SymReference
    template<>
    struct hash<ass::internal::SymReference S> {
    public:
        std::size_t operator()(S const& s) const {
            return std::hash<std::hash<std::string>()(s.ref->name);
        }
    };
} // end std overload

namespace ass { namespace internal {

class FileState {
    std::set<SymReference> importedSyms;
    std::set<SymReference> exportedSyms;
    std::string name;
public:
    FileState(const std::string name);
    bool isImported(const SymReference &) const;
    bool isExported(const SymReference &) const;
    void import(const SymReference &sym) {importedSyms.emplace(sym);}
    void export(const SymReference &sym) {exportedSyms.emplace(sym);}
};


class AssemblingStatus {
    bool error, originDefined;  
    addr_t origin;
    std::shared_ptr<ILogger> logger;
    std::unordered_map<bfs::path, FileState> files;
    std::list<Instruction> insts;
public:
    AssemblingStatus(std::shared_ptr<ILogger> logger);
    std::FileState & getState(const bfs::path & path) {
        if(files.count(path) == 0){
            files.emplace(std::make_pair(path, FileState(path.generic_string())));
        }
        return files[path];
    }

    std::list<instructions> insts & instList() {return insts};

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


};

extern const addr_t DEFAULT_ORIGIN;


/**
 * @brief utility function for removing comments.
 */ 
inline std::string removeComments(const std::string &line) {
    return std::string(line.begin(), std::find(line.begin(), line.end(), ';'));
}


/**
 * @brief Checks all files for ORIGIN directive.
 * @details checks only first line of each file. Resets get pointer if not found.
 * updates state accordingly. default origin if none found is ass::internal::DEFAULT_ORIGIN
 * 
 * @param files vector of pairs of files and paths
 * @param state AssemblingState to update.
 */
void setOrigin(const ifvector_t &files, AssemblingStatus &state);
/**
 * @brief Attempts to parse an export directive line.
 * @details  rewinds get pointer if not successful
 * @param file a pair of a file and a path to read from
 * @param state Current assembling state
 * @param lineNum current line number, incremented if successful.
 * @return true if successful
 */
bool tryExport(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum);
/**
 * @brief Attempts to parse a global directive line.
 * 
 * @param file a pair of a file and path to read from.
 * @param state current assembling state
 * @param lineNum current line number, incremented if successful.
 * @return true if successful
 */
bool tryGlobal(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum);

/**
 * @brief Attempts to parse a line as either a label or instruction or data definition.
 * 
 * @param file a pair of file and path to read from
 * @param state current assembling state.
 * @param lineNum current line number incremented if successful.
 * @return true if successful.
 */
bool parseLine(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum);

}}
