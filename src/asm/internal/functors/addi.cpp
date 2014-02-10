#include "asm/internal/inst_helpers.hpp"


bool ass::internal::AddiF::operator()( const std::string &line,
                                       AssemblingStatus &state,
                                       std::size_t &lineNum, FileState &file ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::arithmImm ) ) {

        try {
            Instruction i;
            auto &insts = state.instList();
            i.type = InstType::ADDI;
            i.data = 0x4000;
            std::uint8_t rd = std::stoi( std::string( result[2] ).substr( 1 ) ) & 0x07;
            std::uint8_t rs = std::stoi( std::string( result[3] ).substr( 1 ) ) & 0x07;
            i.data |= rd << 10;
            i.data |= rs << 7;
            i.pc = insts.size() * 2;

            if ( std::isdigit( std::string( result[4] )[0] ) ) {
                // number !!!!!!!!!!!
                i.data |= i.data |= convertNumber( result[4] ) & 0x7F; // only 7 bits
            } else {
                // label !!!!!!!!!!!
                resolveLabel( result[4], state, file, lineNum, i );
            }

            insts.push_back( i );

        } catch ( ... ) {
            state << "BADD ADDI WHY U DO ZIS?! LINE: " << line
                  << "\nFile: " << file.name.generic_string() << '\n';
        }

        lineNum++;
        return true;
    } else {
        return false;
    }
}

