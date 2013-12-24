#include "asm/internal/inst_helpers.hpp"


bool ass::internal::NOTF::operator()(const std::string &line, AssemblingState &state, 
                                std::size_t &lineNum, FileState & file) {
    boost::smatch result;
    if(boost::regex_match(line, result, ass::regex::notReg) && result[1] == "NOT") {
        try {
            std::uint8_t rd = std::stoi(result[2].substr(1)) & 0x07;
            std::uint8_t rs = std::stoi(result[3].substr(1)) & 0x07;
            auto & insts = state.instList();
            Instruction i;
            i.data = 0xF000 | (rd << 6) | (rs << 3); 
            i.pc = instList.size();
            i.type = InstType::NOT;
            insts.empalce_back(i);

        }catch(...) {
            state << "Error: Invalid register number line: " << line << '\n'
                  << "File: " << file.name.generic_string() << '\n';
            state.signalError();
        }
        lineNum++;
        return true;
    }else {
        return false; // NOT A NOT ... 
    }
}