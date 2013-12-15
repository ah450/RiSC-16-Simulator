#include "asm/assembler.hpp"
#include "asm/internal/regex.hpp"
#include "asm/internal/instructions.hpp"
#include <boost/filesystem/fstream.hpp>
#include <boost/algorithm/string.hpp>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <cstdint>
#include <utility>
#include <string>
#include <memory>
#include <list>

namespace ass { namespace internal {


namespace bfs = boost::filesystem;

typedef std::uint16_t addr_t;
typedef std::uint16_t pc_t;
typedef std::pair<std::unique_ptr<bfs::ifstream>, bfs::path> file_path_pair_t;
typedef std::vector<file_path_pair_t> ifvector_t;
enum class SymType {INST, DATA};

struct Sym {
    bool defined;
    bool global;
    addr_t address;
    SymType type;
    std::string name;
    bfs::path file;
    std::size_t lineNum;
};

struct FileState {
    std::list<Sym> importedSyms;
    std::list<Sym> exportedSyms;
};


struct AssemblingStatus {
    bool error, originDefined;
    bool entry;
    addr_t origin;
    std::unordered_map<std::string, Sym> symbols;
    std::unordered_multimap<std::string, std::shared_ptr<Instruction>> incompleteInstructions;
    std::vector<std::shared_ptr<Instruction>> instructions;
    ILogger * logger;
    FileState currentFileState;
    AssemblingStatus(ILogger * logger) : error(false), originDefined(false), symbols(), logger(logger){}
    void resetCurrentFile(){currentFileState = FileState();} 


};

extern const addr_t DEFAULT_ORIGIN;


inline std::striing removeComments(const std::string &line) {
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
