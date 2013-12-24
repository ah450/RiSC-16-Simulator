#include "asm/internal/internal.hpp"
#include <boost/tokenizer.hpp>
#include <iterator>
#include <string>
using namespace ass::internal;

const addr_t ass::internal::DEFAULT_ORIGIN = 0;


static bool parseLabel(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum);
static bool parseInstruct(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum);


inline std::vector<std::string> tokenizeLabels(const std::string &list) {
    boost::char_separator<char> sep(",");
    boost::tokenizer<boost::char_separator<char>> tok(list, sep);
    return std::vector<std::string>(tok.begin(), tok.end());
}

inline std::string removeComments(const std::string &orig) {
    return std::string(orig.begin(),  std::find(orig.begin(), orig.end(), ';'));
}




void ass::internal::setOrigin(const ifvector_t &files, AssemblingStatus &state) {

}


bool ass::internal::tryExport(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {
    return true;
}

bool ass::internal::tryGlobal(file_path_pair_t &file, AssemblingStatus &state, std::size_t &lineNum) {
    return true;
   
}




