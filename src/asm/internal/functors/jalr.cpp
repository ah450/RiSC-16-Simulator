#include "asm/internal/inst_helpers.hpp"


bool ass::internal::JALRF::operator()( const std::string &line,
                                       AssemblingStatus &state,
                                       std::size_t &lineNum, FileState &file ) {
    boost::smatch result;

    if ( boost::regex_match( line, result, ass::regex::jalReg ) ) {
        try {
            Instruction i;
            auto &insts = state.instList();
            i.type = InstType::JALR;
            i.data = 0xA000; // opcode
            std::uint8_t rl = std::stoi( std::string( result[2] ).substr( 1 ) ) & 0x07;
            i.data |= rl << 10;
            std::uint8_t rd = std::stoi( std::string( result[3] ).substr( 1 ) ) & 0x07;
            i.data |= rd << 7;
            i.pc = insts.size() * 2;
            insts.push_back( i );
        } catch ( ... ) {
            state << "BAD REGISTER NUMBER OR SOMETHING WENT HORRIBLY WRONG !!!!!! LINE: "
                  << line << "\nFILE: " << file.name.generic_string();
            state.signalError();
        }

        lineNum++;
        return true;
    } else {
        return false;
    }
}