#include "asm/internal/inst_helpers.hpp"

bool ass::internal::XORF::operator()( const std::string &line,
                                      AssemblingStatus &state,
                                      std::size_t &lineNum, FileState &file ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::arithmReg )
            && result[1] == "XOR" ) {
        try {
            auto regs = getRegsFromArithm( result );
            auto &insts = state.instList();
            Instruction i;
            i.type = InstType::XOR;
            i.pc = insts.size() * 2;
            i.data = 0xEE00 | unpackRegs( regs );
            insts.emplace_back( i );
        } catch ( ... ) {
            state << "Error: Invalid register number line: " << line << '\n'
                  << "File: " << file.name.generic_string() << '\n';
            state.signalError();
        }

        lineNum++;
        return true;
    } else {
        return false; // not an XOR instruction
    }
}