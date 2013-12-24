#include "asm/internal/inst_helpers.hpp"


bool ass::internal::MulF::operator()(const std::string &line, AssemblingState &state,
            std::size_t &lineNum, FileState & file) {
    boost::smatch result;
    if(boost::regex_match(line, result, ass:regex::arithmReg) && result[1] == "MUL") {
        try {
            auto regs = getRegsFromArithm(result);
            auto instList & = state.insts();
            Instruction i;
            i.type = InstType::MUL;
            i.pc = instList.size();
            i.data = 0xE400 | unpackRegs(regs);
            instList.empalce_back(i);
        }catch (...) {
            state << "Error: Invalid register number line : " << line << '\n'
                  << "File: " << file.name.generic_string() << '\n';

            state.signalError()
        }
         lineNum++;
        return true;
    }else {
        return false;
    }
}