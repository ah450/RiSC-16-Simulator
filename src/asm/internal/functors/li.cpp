#include "asm/internal/inst_helpers.hpp"


bool ass::internal::LIF::operator()( const std::string &line,
                                     AssemblingStatus &state,
                                     std::size_t &lineNum, FileState &file ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::loadImm ) ) {
        try {
            Instruction i;
            i.type = InstType::LI;
            auto &insts = state.instList();
            i.pc = insts.size() * 2 ;
            std::uint8_t rd = std::stoi( std::string( result[2] ).substr( 1 ) ) & 0x07;
            i.data = 0xF400;
            i.data |= rd << 6;

            if ( std::isdigit( std::string( result[3] )[0] ) ) {
                // number
                i.data |= convertNumber( result[3] ) & 0x3F;
            } else {
                resolveLabel( result[3], state, file, lineNum, i );
            }

            insts.push_back( i );
        } catch ( ... ) {
            state << "Invalid register or immediate value Line : " << line
                  << "\nFile: " << file.name.generic_string() << '\n';
            state.signalError();
        }

        lineNum++;
        return true;
    } else {
        return false;
    }
}
