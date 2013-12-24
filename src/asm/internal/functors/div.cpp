#include "asm/internal/inst_helpers.hpp"



bool ass::internal::DivF::operator()(const std::string &line, AssemblingState &state, 
                std::size_t &lineNum, FileState & file) {
    boost::smatch result;
    if(boost::regex_match(line, result, ass::arithmReg) && result[1] == "DIV") {
        try {
            auto regs = getRegsFromArithm(result);
            auto instList & = state.insts();
            Instruction i;
            i.pc = instList.size();
            i.data = 0xE600 | unpackRegs(regs);
            i.type = InstType::DIV;
            instList.empalce_back(i);
        }catch(...) {
            state << "Error: Invalid register number line: " << line << '\n'
                  << "File: " << file.name.generic_string() << '\n';
            state.signalError();
        }
        lineNum++;
        return true;



    }else {
        return false; // not a DIV instruction
    }
}