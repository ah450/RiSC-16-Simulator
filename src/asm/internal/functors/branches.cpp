#include "asm/internal/inst_helpers.hpp"

bool ass::internal::BEQF::operator()( const std::string &line,
                                      ass::internal::AssemblingStatus &state,
                                      std::size_t &lineNum, ass::internal::FileState &file ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::branch ) ) {

        try {
            Instruction i;
            auto &insts = state.instList();
            i.pc = insts.size() * 2;
            std::string type( result[1] );
            i.type = InstType::BEQ;
            i.data = 0x8000;
            std::uint8_t rd = std::stoi( std::string( result[2] ).substr( 1 ) );
            std::uint8_t rs = std::stoi( std::string( result[3] ).substr( 1 ) );
            i.data |= rd << 10;
            i.data |= rs << 7;

            if ( std::isdigit( std::string( result[4] )[0] ) ) {
                /**********************************
                 * Labels Don't start with numbers*
                 **********************************/

                i.data |= convertNumber( result[4] ) & 0x7F; // only 7 bits
            } else {

                /*****************
                 * Resolve Label *
                 *****************/
                resolveLabel( result[4], state, file, lineNum, i );
            }

            insts.push_back( i );
        } catch ( ... ) {
            state << "OMG NO NO NO BAD PROGRAMMER ! Line: " << line
                  << "\nFile: " << file.name.generic_string();
        }

        lineNum++;
        return true;
    } else {
        return false;
    }
}

