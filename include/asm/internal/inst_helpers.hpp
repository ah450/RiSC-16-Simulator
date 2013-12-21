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
        std::uint8_t rd = std::stoi(m[2].substr(1)) & 0x07;
        std::uint8_t rs = std::stoi(m[3].substr(1)) & 0x07;
        std::uint8_t rt = std::stoi(m[4].substr(1)) & 0x07;
        return std::make_tuple(rd, rs, rt); 
    }
    inline std::uint16_t unpackRegs(std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> &regs) {
        return (regs.get<0>() << 6) | (regs.get<1>() << 3) | regs.get<2>();
    }
    struct HaltF {

        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum) {
            boost::smatch result;
            if(boost::regex_match(line, result, ass::regex::halt)) {
                Instruction i;
                auto instList & = state.insts();
                i.type = InstType::HALT;
                i.pc = instList.size();
                i.data = 0xE800;
                instList.empalce_back(i);
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
                auto regs = getRegsFromArithm(result);
                auto instList & = state.insts();
                Instruction i;
                i.type = InstType::ADD;
                i.pc = instList.size();
                i.data = (0xE000) | unpackRegs(regs) ;
                instList.empalce_back(i);
               
            }catch(...) {
                state << "Error: Invalid register number : " << line << '\n';
                state.signalError()
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
                auto regs = getRegsFromArithm(result);
                auto instList & = state.insts();
                Instruction i;
                i.type = InstType::MUL;
                i.pc = instList.size();
                i.data = 0xE400 | unpackRegs(regs);
                instList.empalce_back(i);
            }catch (...) {
                state << "Error: Invalid register number line : " << line << '\n';
                state.signalError()
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

                auto regs = getRegsFromArithm(result);
                auto instList & = state.insts();
                Instruction i;
                i.pc = instList.size();
                i.data = 0xE200 | unpackRegs(regs);
                i.type = InstType::SUB;
                instList.empalce_back(i);

            }catch(...) {
                state << "Error: Invalid register number line: " << line << '\n';
                state.signalError();
                
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
                auto regs = getRegsFromArithm(result);
                auto instList & = state.insts();
                Instruction i;
                i.pc = instList.size();
                i.data = 0xE600 | unpackRegs(regs);
                i.type = InstType::DIV;
                instList.empalce_back(i);
            }catch(...) {
                state << "Error: Invalid register number line: " << line << '\n';
                state.signalError();
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
                auto regs = getRegsFromArithm(result);
                auto instList & = state.insts();
                Instruction i;
                i.data = 0xEA00 | unpackRegs(regs);
                i.pc = instList.size();
                i.type = InstType::AND;
                instList.empalce_back(i);

            }catc(...) {
                state << "Error: Invalid register number line: " << line << '\n';
                state.signalError(); 
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
                    auto instList & = state.insts();
                    Instruction i;
                    i.data = 0xEC00 | unpackRegs(regs);
                    i.pc = instList.size();
                    i.type = InstType::OR;
                    instList.empalce_back(i);
                }catch(...) {
                    state << "Error: Invalid register number line: " << line << '\n';
                    state.signalError();
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
                    auto & insts = state.instList();
                    Instruction i;
                    i.type = InstType::OR;
                    i.pc = insts.size();
                    i.data = 0xEE00 | unpackRegs(regs);
                    insts.empalce_back(i);
                }catch(...) {
                    state << "Error: Invalid register number line: " << line << '\n';
                    state.signalError();
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
            if(boost::regex_match(line, result, ass::regex::notReg) && result[1] == "NOT") {
                try {
                    std::uint8_t rd = std::stoi(result[2].substr(1)) & 0x07;
                    std::uint8_t rs = std::stoi(result[3].substr(1)) & 0x07;
                    auto & insts = state.instList();
                    Instruction i;
                    i.data = 0xF000 | (rd << 6) | (rs << 3); 
                    i.pc = instList.size();
                    i.type = InstType::NOT;
                    state.instructions.empalce_back(i);

                }catch(...) {
                    state << "Error: Invalid register number line: " << line << '\n';
                    state.signalError();
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
                i.pc = instList.size();
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
                i.pc = instList.size();
                state.instructions.empalce_back(std::make_shared(new Instruction(i)));
                lineNum++;
                return true;
            }else {
                return false; // not an ADDI instruction 
            }
        }
    };
    std::array<instFunct_t, > instParsers = {{
        {HaltF()}, {AddF()}, {MulF()}, {SubF()}, {ORF()}, {XORF()}, {LSWF()}, {AddiF()},
        {NOTF()}, {AndF()}, {DivF()}
    
    }};
}