#include "ass/internal/inst_helpers.hpp"



bool ass::internal::HaltF::operator()(const std::string &line, AssemblingStatus &state, 
    std::size_t &lineNum, FileState & file) {
    boost::smatch result;
    if(boost::regex_match(line, result, ass::regex::halt)) {
        Instruction i;
        auto instList & = state.insts();
        i.type = InstType::HALT;
        i.pc = instList.size();
        i.data = 0xE800;
        instList.push_back(i);
        lineNum++;
        return true;
    }else {
        return false;
    }
}
