#pragma once
#include "asm/assembler.hpp"
#include "asm/internal/regex.hpp"
#include "asm/internal/typedefs.hpp"
#include "asm/internal/sym.hpp"
#include "asm/internal/sym_reference.hpp"
#include "asm/internal/instruction.hpp"
#include "asm/internal/file_state.hpp"
#include "asm/internal/assembling_state.hpp"
#include "asm/internal/utility.hpp"
#include <boost/filesystem.hpp>


namespace ass {
namespace internal {


namespace bfs = boost::filesystem;



extern const addr_t DEFAULT_ORIGIN;




/**
 * @brief Checks all files for ORIGIN directive.
 * @details checks only first line of each file. Resets get pointer if not found.
 * updates state accordingly. default origin if none found is ass::internal::DEFAULT_ORIGIN
 *
 * @param files vector of pairs of files and paths
 * @param state AssemblingStatus to update.
 */
void setOrigin( const ifvector_t &files, AssemblingStatus &state );
/**
 * @brief Attempts to parse an export directive line.
 * @details  rewinds get pointer if not successful
 * @param file a pair of a file and a path to read from
 * @param state Current assembling state
 * @param lineNum current line number, incremented if successful.
 * @return true if successful
 */
void tryExport( file_path_pair_t &file, AssemblingStatus &state,
                std::size_t &lineNum );
/**
 * @brief Attempts to parse a global directive line.
 *
 * @param file a pair of a file and path to read from.
 * @param state current assembling state
 * @param lineNum current line number, incremented if successful.
 * @return true if successful
 */
void tryGlobal( file_path_pair_t &file, AssemblingStatus &state,
                std::size_t &lineNum );

/**
 * @brief Attempts to parse a line as either a label or instruction or data definition.
 *
 * @param file a pair of file and path to read from
 * @param state current assembling state.
 * @param lineNum current line number incremented if successful.
 * @return true if successful.
 */
bool parseLine( const std::string &line , AssemblingStatus &state,
                FileState &fs, std::size_t &lineNum );

}
}
