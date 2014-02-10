
#include "asm/internal/inst_helpers.hpp"



bool ass::internal::AndF::operator()( const std::string &line,
                                      AssemblingStatus &state,
                                      std::size_t &lineNum, FileState &file ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::arithmReg )
            && result[1] == "AND" ) {
        try {
            auto regs = getRegsFromArithm( result );
            auto &instList = state.instList();
            Instruction i;
            i.data = 0xEA00 | unpackRegs( regs );
            i.pc = instList.size() * 2;
            i.type = InstType::AND;
            instList.emplace_back( i );

        } catch ( ... ) {
            state << "Error: Invalid register number line: " << line << '\n'
                  << "File: " << file.name.generic_string() << '\n' ;
            state.signalError();
        }

        lineNum++;
        return true;
    } else {
        return false; // not an AND instruction
    }
}