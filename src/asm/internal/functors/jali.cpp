#include "asm/internal/inst_helpers.hpp"

bool ass::internal::JALIF::operator()( const std::string &line,
                                       AssemblingStatus &state,
                                       std::size_t &lineNum, FileState &file ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::jalImm ) ) {

        try {
            Instruction i;
            auto &insts = state.instList();
            i.pc = insts.size() * 2;
            i.type = InstType::JALI;
            i.data = 0xC000;
            std::uint8_t rl = std::stoi( std::string( result[2] ).substr( 1 ) ) & 0x07;
            i.data |= rl << 10;

            if ( std::isdigit( std::string( result[3] )[0] ) ) {
                i.data |=  ( convertNumber( result[3] ) & 0x03FF ); // 10 bits
            } else {
                resolveLabel( result[3], state, file, lineNum, i );
            }

            insts.push_back( i );
        } catch ( ... ) {
            state << "ERROR PARSING LINE: " << line
                  << "\nFILE: " << file.name.generic_string();
            state.signalError();
        }

        lineNum++;
        return true;
    } else {
        return false;
    }
}