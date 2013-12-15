#pragma once
#include "internal.hpp"
#include <array>

//TODO:GENERATE OPCODES
namespace {
    typedef std::function<bool(const std::string &, AssemblingState &, std::size_t&) instFunct_t;
    using ass::internal;
    struct HaltF {

        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
            boost::smatch result;
            if(boost::regex_match(line, result, ass::regex::halt)) {
                Instruction i;
                i.type = InstType::HALT;
                i.pc = state.instructions.size();
                i.data = 0; // FIXME
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));
                lineNum++;
                return true;
            }else {
                return false;
            }
        }
    };

    struct AddF {
    bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
        boost::smatch result;
        if(boost::regex_match(line, result, ass::regex::arithmReg) && result[1] == "ADD") {
            try {

                std::uint8_t rd = std::stoi(result[2].substr(1));// drop R
                std::uint8_t rs = std::stoi(result[3].substr(1));
                std::uint8_t rt = std::stoi(result[4].substr(1));
                Instruction i;
                i.type = InstType::ADD;
                i.pc = state.instructions.size();
                i.data = 0; // FIXME
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));
                lineNum++;
                return true;
            }catch (std::invalid_argument &ia) {
                //FIXME: Well there is no way the regex matched and its an invalid argumen
                // assuming  that  the regex is correct.

            }catch(std::out_of_range &oor) {
                (*state.logger) << "Invalid register number : " << line << '\n';
            }
            state.error = true;
            lineNum++;
            return true;
        }else {
            return false;
        }
    } 
    };

    struct MulF {

    bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
        boost::smatch result;
        if(boost::regex_match(line, result, ass:regex::arithmReg) && result[1] == "MUL") {
            try {
                std::uint8_t rd = std::stoi(result[2].substr(1));// drop R
                std::uint8_t rs = std::stoi(result[3].substr(1));
                std::uint8_t rt = std::stoi(result[4].substr(1));
                Instruction i;
                i.type = InstType::MUL;
                i.pc = state.instructions.size();
                i.data = 0; //FIXME
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));
                lineNum++;
                return true;
            }catch (...) {
                (*state.logger) << "Invalid register number line : " << line; << '\n';
                state.error = true;
                lineNum++;
                return true;
            }
        }else {
            return false;
        }
    }
    };

    struct SubF{
    bool
    };



    std::array<instFunct_t, > instParsers = {{
        {HaltF()}, {AddF()}, {MulF()}
    
    }};
}