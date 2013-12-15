#pragma once
#include "internal.hpp"
#include <functional>
#include <memory>
#include <array>

//TODO:GENERATE OPCODES
namespace {
    typedef std::function<bool(const std::string &, AssemblingState &, std::size_t&) instFunct_t;
    using ass::internal;

    inline std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> getRegsFromArithm(boost::smatch &m) {
        std::uint8_t rd = std::stoi(m[2].substr(1));
        std::uint8_t rs = std::stoi(m[3].substr(1));
        std::uint8_t rt = std::stoi(m[4].substr(1));
        return std::make_tuple(rd, rs, rt); 
    }
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
               
            }catch(...) {
                (*state.logger) << "Error: Invalid register number : " << line << '\n';
                state.error = true;
            }
            
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
                auto regsTuple = getRegsFromArithm(result);
                Instruction i;
                i.type = InstType::MUL;
                i.pc = state.instructions.size();
                i.data = 0; //FIXME
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));
            }catch (...) {
                (*state.logger) << "Error: Invalid register number line : " << line << '\n';
                state.error = true;
            }
             lineNum++;
            return true;
        }else {
            return false;
        }
    }
    };

    struct SubF{
     bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
        boost::smatch result;
        if(boost::match_result(line, result, ass::regex::arithmReg) && result[1] == "SUB") {
            try {

                auto regsTuple = getRegsFromArithm(result);
                Instruction i;
                i.pc = state.instructions.size();
                i.data = 0;// FIXME
                i.type = InstType::SUB;
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));

            }catch(...) {
                (*state.logger) << "Error: Invalid register number line: " << line << '\n';
                state.error = true;
                
            }
            lineNum++;
            return true;

        }else {
            return false; // not a SUB instruction
        }
     }  
    };

    struct DivF{

    bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
        boost::smatch result;
        if(boost::regex_match(line, result, ass::arithmReg) && result[1] == "DIV") {
            try {
                auto regsTuple = getRegsFromArithm(result);
                Instruction i;
                i.pc = state.instructions.size();
                i.data = 0; // FIXME
                i.type = InstType::DIV;
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));
            }catch(...) {
                (*state.logger) << "Error: Invalid register number line: " << line << '\n';
                state.error = true;
            }
            lineNum++;
            return true;



        }else {
            return false; // not a DIV instruction
        }
    }
    };


    struct AndF {
    bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
        boost::smatch result;
        if(boost::regex_match(line, result, ass::arithmReg) && result[1] == "AND") {
            try {
                auto regsTuple = getRegsFromArithm(result);
                Instruction i;
                i.data = 0; // FIXME
                i.pc = state.instructions.size();
                i.type = InstType::AND;
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));

            }catc(...) {
                (*state.logger) << "Error: Invalid register number line: " << line << '\n';
                state.error = true; 
            }
            lineNum++;
            return true;
        }else {
            return false; // not an AND instruction
        }
    }
    };

    struct ORF {
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
            boost::smatch result;
            if(boost::regex_match(line, result, ass::regex::arithmReg) && result[1] == "OR") {
                try {
                    auto regs = getRegsFromArithm(result);
                    Instruction i;
                    i.data = 0;// FIXME
                    i.pc = state.instructions.size();
                    i.type = InstType::OR;
                    state.instructions.empalce_back(std::make_shared(new Instruction(i)));
                }catch(...) {
                    (*state.logger) << "Error: Invalid register number line: " << line << '\n';
                    state.error = true;
                }
                lineNum++;
                return true;
            }else {
                return false; // not an OR instruction
            }
        }

    };

    struct XORF {
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
            boost::smatch result;
            if(boost::regex_match(line, result, ass::regex::arithmReg) && result[1] == "XOR") {
                try {
                    auto regs = getRegsFromArithm(result);
                    Instruction i;
                    i.type = InstType::OR;
                    i.pc = state.instructions.size();
                    i.data = 0; // FIXME
                    state.instructions.empalce_back(std::make_shared(new Instruction(i));
                }catch(...) {
                    (*state.logger) << "Error: Invalid register number line: " << line << '\n';
                    state.error = true;
                }
                lineNum++;
                return true;
            }else {
                return false; // not an XOR instruction
            }
        }
    };

    struct NOTF{
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
            boost::smatch result;
            if(boost::regex_match(line, result, ass::regex::arithmReg) && result[1] == "NOT") {

                try {
                    auto regs = getRegsFromArithm(result);
                    Instruction i;
                    i.data = 0; //FIXME
                    i.pc = state.instructions.size();
                    i.type = InstType::NOT;
                    state.instructions.empalce_back(new Instruction(i));

                }catch(...) {
                    (*state.logger) << "Error: Invalid register number line: " << line << '\n';
                    state.error = true;
                }
                lineNum++;
                return true;
            }else {
                return false; // NOT A NOT ... 
            }
        }
    };

    struct LSWF{
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
            boost::smatch result;
            if(boost::regex_match(line, result, ass::regex::loadStore)) {
                Instruction i;
                if(result[1] == "LW") {
                    i.type = InstType::LW;
                }else{
                    i.type == InstType::SW;
                }
                i.data = 0;
                i.pc = state.instructions.size();
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));
                lineNum++;
                return true;
            }else {
                return false;
            }
        }
    };

    struct AddiF {
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum){
            boost::smatch result;
            if(boost::regex_match(line, result, ass::regex::arithmImm) && result[1] == "ADDI") {
                Instruction i;
                i.data = 0; //FIXME 
                i.type = InstType::ADDI;
                i.pc = state.instructions.size();
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));
                lineNum++;
                return true;
            }else {
                return false; // not an ADDI instruction 
            }
        }
    };
    std::array<instFunct_t, > instParsers = {{
        {HaltF()}, {AddF()}, {MulF()}, {SubF()}, {ORF()}, {XORF()}, {LSWF()}, {AddiF()}
    
    }};
}