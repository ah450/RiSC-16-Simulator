#include "asm/internal/internal.hpp"
using namespace ass::internal;


void ass::internal::setOrigin(const ifvector_t &files, AssemblingStatus &state) {

    using namespace ass;
    boost::smatch result;
    for(auto &pair : files) {
        auto originalGet = pair.first->tellg();
        std::string line;
        // read line
        std::getline(*(pair.first), line);
        boost::algorithm::trim(line);
        // attemp to match line
        if(boost::regex_match(line, result, regex::originDirect)) {
            // regex matched
            if(state.originDefined) {
                // origin can only be defined once per program
                (*state.logger) << "Error : origin directory already redefinition encountered while processing file: "
                    << pair.second.generic_string();
                state.error = true;
            }else {
                // first time origin directive encountered
               try {
                std::string addr = result[2];
                boost::trim(addr);
                int base = 10;
                // detect base
                if(addr.size() > 2) {
                    if(addr[0] == '0' && addr[1] == 'x'){
                        // base 16
                        base = 16;
                    }else if(addr[0] == '0' && addr[1] == 'b') {
                        // binary
                        base = 2;
                        addr = addr.substr(2); // skip 0b prefix
                    }
                } 
                // convert value   
                int value = std::stoi(addr, nullptr, base);
                // range checks
                if(value < 0) {
                    //negative origin
                    (*state.logger) << "Error : Invalid origin directive - address can not be negative - in file: "
                                    << pair.second.generic_string();
                    state.error = true;

                }else if((value & 0x0000) != 0) {

                    throw std::out_of_range(std::to_string(value));
                }else {
                    // all checks passed
                    state.origin = value;
                    state.originDefined = true;
                }

               }catch (std::invalid_argument &ia) {
                    (*state.logger) << "Error : Could not parse paramater of origin directive in file: "
                                    << pair.second.generic_string();
                    state.error = true;
               }catch (std::out_of_range &e) {
                    (*state.logger) << "Error : Origin paramater to large for 16 bits in file: "
                                    << pair.second.generic_string();
                    state.error = true;
               } 

            }
        }else {
            // no origin directive. reset get pointer to beggining of file
            pair.first->seekg(originalGet);
        }
    } // end for each

    //default value
    if(!state.originDefined) {
        state.originDefined = true;
        state.origin = DEFAULT_ORIGIN;
    }


}