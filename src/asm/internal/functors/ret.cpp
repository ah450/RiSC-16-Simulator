#include "asm/internal/inst_helpers.hpp"


bool ass::internal::RETF::operator()( const std::string &line,
                                      AssemblingStatus &state,
                                      std::size_t &lineNum, FileState &file ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::ret ) ) {
        Instruction i;

        try {
            auto &insts = state.instList();
            i.pc = insts.size() * 2;
            i.type = ass::internal::InstType::RET;
            std::uint8_t rd = std::stoi( std::string( result[2] ).substr( 1 ) ) & 0x07;
            i.data = 0xF200;
            i.data |= rd << 6;
            insts.push_back( i );
        } catch ( ... ) {
            state << "OMG BAD RET INSTRUCTION WTF?!?!?!? LINE: " << line
                  << "\nFile: " << file.name.generic_string() << '\n';
            state.signalError();
        }

        lineNum++;
        return true;
    } else {
        return false;
    }
}