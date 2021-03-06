#include "asm/internal/inst_helpers.hpp"

bool ass::internal::JMPF::operator()( const std::string &line,
                                      AssemblingStatus &state,
                                      std::size_t &lineNum, FileState &file ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::jmp ) ) {

        Instruction i;
        auto &insts = state.instList();
        i.type = InstType::JMP;
        i.pc = insts.size() * 2;
        i.data = 0x6000; // opcode

        try {
            std::uint8_t rs = std::stoi( std::string( result[2] ).substr( 1 ) ) & 0x07;
            i.data |= rs << 10;

            if ( std::isdigit( std::string( result[3] )[0] ) ) {
                i.data |=  ( convertNumber( result[3] ) & 0x3FF ); // 10 bits
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