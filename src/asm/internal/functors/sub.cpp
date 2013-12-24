#include "asm/internal/inst_helpers.hpp"


bool ass::internal::SubF::operator()(const std::string &line, AssemblingState &state, 
    std::size_t &lineNum, FileState & file) {

    boost::smatch result;
        if(boost::match_result(line, result, ass::regex::arithmReg) && result[1] == "SUB") {
            try {

                auto regs = getRegsFromArithm(result);
                auto instList & = state.insts();
                Instruction i;
                i.pc = instList.size();
                i.data = 0xE200 | unpackRegs(regs);
                i.type = InstType::SUB;
                instList.empalce_back(i);

            }catch(...) {
                state << "Error: Invalid register number line: " << line << '\n'
                      << "File: " << file.name.generic_string() << '\n';
                state.signalError();
                
            }
            lineNum++;
            return true;

        }else {
            return false; // not a SUB instruction
        }

}