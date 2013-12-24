#include "asm/internal/inst_helpers.hpp"

using namespace ass::internal;
static bool parseInstruction(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum);
static bool parseLabel(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum);

bool ass::internal::parseLine(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {
    return parseInstruction(file, state, lineNum) || parseLabel(file, state, lineNum);
}


static bool parseInstruction(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {
    auto originalGet = file.first->tellg();
    std::string line;
    std::getline(*file.first, line);
    line = removeComments(line);
    boost::trim(line);
    return true;


}
static bool parseLabel(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {
    return true;
}


