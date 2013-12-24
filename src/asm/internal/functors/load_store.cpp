#include "asm/internal/inst_helpers.hpp"



bool ass::internal::LSWF::operator()(const std::string &line, AssemblingState &state, 
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
                i.type == InstType::SW;
                i.data = 0x2000; // SW opcode
            }

            i.pc = instList.size();

            /*******************
             * SET RD and RS   *
             *******************/

            std::uint8_t rd = std::stoi(results[2]) & 0x07;
            std::uint8_t rs = std::stoi(results[3]) & 0x07;

            i.data |= rd << 10;
            i.data |= rs << 7;


            if(std::isdigit(result[4][0]) {
                /**********************************
                 * Labels Don't start with numbers*
                 **********************************/

                data |= convertNumber(result[4]) & 0x07; // only 7 bits
            }else {
                
                /*****************
                 * Resolve Label *
                 *****************/ 
                 ok = resolveLabel(result[4], state, file, lineNum, i);
            }
            
            insts.empalce_back(i);
            return ok;
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