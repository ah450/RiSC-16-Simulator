#include "asm/internal/inst_helpers.hpp"



bool ass::internal::LSWF::operator()(const std::string &line, AssemblingStatus &state, 
                        std::size_t &lineNum, FileState & file) {
    boost::smatch result;
    if(boost::regex_match(line, result, ass::regex::loadStore)) {
        bool ok = true;
        try {    
            Instruction i;
            auto & insts = state.instList();
            /**************
             * Set Opcode *
             **************/ 
            if(result[1] == "LW") {
                i.type = InstType::LW;
                i.data = 0; // LW opcode                  
            }else{
                i.type = InstType::SW;
                i.data = 0x2000; // SW opcode
            }

            i.pc = insts.size();

            /*******************
             * SET RD and RS   *
             *******************/

            std::uint8_t rd = std::stoi(result[2]) & 0x07;
            std::uint8_t rs = std::stoi(result[3]) & 0x07;

            i.data |= rd << 10;
            i.data |= rs << 7;


            if(std::isdigit(std::string(result[4])[0]) ) {
                /**********************************
                 * Labels Don't start with numbers*
                 **********************************/

                i.data |= convertNumber(result[4]) & 0x07; // only 7 bits
            }else {
                
                /*****************
                 * Resolve Label *
                 *****************/ 
                 resolveLabel(result[4], state, file, lineNum, i);
            }
            
            insts.emplace_back(i);
            return true;
        }catch(...) {
            state << "Error : Invalid register number line: " << line << '\n'
                  << "File: " << file.name.generic_string() << '\n';
            state.signalError();
        }
        lineNum++;
        return true;
    }else {
        return false;
    }
}