#include "ass/internal/inst_helpers.hpp"



bool ass::internal::AndF::operator()(const std::string &line, AssemblingStatus &state, 
                        std::size_t &lineNum, FileState & file) {
    boost::smatch result;
    if(boost::regex_match(line, result, ass::arithmReg) && result[1] == "AND") {
        try {
            auto regs = getRegsFromArithm(result);
            auto instList & = state.insts();
            Instruction i;
            i.data = 0xEA00 | unpackRegs(regs);
            i.pc = instList.size();
            i.type = InstType::AND;
            instList.empalce_back(i);

        }catc(...) {
            state << "Error: Invalid register number line: " << line << '\n'
                  << "File: " << file.name.generic_string() << '\n' ;
            state.signalError(); 
        }
        lineNum++;
        return true;
    }else {
        return false; // not an AND instruction
    }
}