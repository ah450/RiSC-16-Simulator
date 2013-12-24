#include "asm/internal/inst_helpers.hpp"

bool ass::internal::ORF::operator()(const std::string &line, AssemblingState &state, 
                                std::size_t &lineNum, FileState & file) {
    boost::smatch result;
    if(boost::regex_match(line, result, ass::regex::arithmReg) && result[1] == "OR") {
        try {
            auto regs = getRegsFromArithm(result);
            auto instList & = state.insts();
            Instruction i;
            i.data = 0xEC00 | unpackRegs(regs);
            i.pc = instList.size();
            i.type = InstType::OR;
            instList.empalce_back(i);
        }catch(...) {
            state << "Error: Invalid register number line: " << line << '\n'
                  << "File: " << file.name.generic_string() << '\n';
            state.signalError();
        }
        lineNum++;
        return true;
    }else {
        return false; // not an OR instruction
    }
}