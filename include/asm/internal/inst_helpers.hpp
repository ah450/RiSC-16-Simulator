#pragma once
#include "internal.hpp"
#include <functional>
#include <memory>
#include <array>

//TODO:GENERATE OPCODES
namespace {
    using namespace ass::internal;
    typedef std::function<bool(const std::string &, AssemblingState &, std::size_t&, FileState&) instFunct_t;


    inline std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> getRegsFromArithm(boost::smatch &m) {
        std::uint8_t rd = std::stoi(m[2].substr(1)) & 0x07;
        std::uint8_t rs = std::stoi(m[3].substr(1)) & 0x07;
        std::uint8_t rt = std::stoi(m[4].substr(1)) & 0x07;
        return std::make_tuple(rd, rs, rt); 
    }
    inline std::uint16_t unpackRegs(std::tuple<std::uint8_t, std::uint8_t, std::uint8_t> &regs) {
        return (regs.get<0>() << 6) | (regs.get<1>() << 3) | regs.get<2>();
    }

    inline std::uint16_t convertNumber(const std::string &num) {

        if(num.size() >=2 num[0] == '0' && num[1] == 'x') {
            // hex
            return std::stoi(num.substr(2), nullptr , 16); 
        }else if(num.size() >=2 num[0] == '0' && num[1] == 'b') {
            return std::stoi(num.substr(2), nullptr , 2); 
        }else {
            // base ten
            return std::stoi(num);
        }
    }

        template<Container>
    inline std::pair<addr_t, bool> getLabelAddr(Container && symList, const std::string & value, pc_t inst) {
        
        for(auto r : symList){
            if(r.ref->name == value) {
                if(r.ref->defined) {
                    return {r.ref->address, true};
                }else {
                    // undefined
                    //TODO: Implement
                }
            }
        }
        return std::pair<addr_t, bool>(0, false);
    }


    struct HaltF {

        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
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
    bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
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
                state << "Error: Invalid register number : " << line << '\n'
                      << "File: " << file.name.generic_string() << '\n';
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

    bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
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
                state << "Error: Invalid register number line : " << line << '\n'
                      << "File: " << file.name.generic_string() << '\n';

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
     bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
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
                state << "Error: Invalid register number line: " << line << '\n'
                      << "File: " << file.name.generic_string() << '\n';
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

    bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
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
                state << "Error: Invalid register number line: " << line << '\n'
                      << "File: " << file.name.generic_string() << '\n';
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
    bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
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
                state << "Error: Invalid register number line: " << line << '\n'
                      << "File: " << file.name.generic_string() << '\n' ;
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
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
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
                    state << "Error: Invalid register number line: " << line << '\n'
                          << "File: " << file.name.generic_string() << '\n';
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
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
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
                    state << "Error: Invalid register number line: " << line << '\n'
                          << "File: " << file.name.generic_string() << '\n';
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
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
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
                    insts.empalce_back(i);

                }catch(...) {
                    state << "Error: Invalid register number line: " << line << '\n'
                          << "File: " << file.name.generic_string() << '\n';
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
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file) {
            boost::smatch result;
            if(boost::regex_match(line, result, ass::regex::loadStore)) {
                try {    
                    Instruction i;
                    auto & insts = state.instList();
                    if(result[1] == "LW") {
                        i.type = InstType::LW;
                        i.data = 0; // LW opcode                  
                    }else{
                        i.type == InstType::SW;
                        i.data = 0x2000; // SW opcode
                    }
                    i.pc = instList.size();
                    std::uint8_t rd = std::stoi(results[2]) & 0x07;
                    std::uint8_t rs = std::stoi(results[3]) & 0x07;
                    i.data |= rd << 10;
                    i.data |= rs << 7;
                    if(std::isdigit(result[4][0]) {
                        // immediate is number
                        data |= convertNumber(result[4]) & 0x07; // only 7 bits
                    }else {
                        // immediate is label
                        auto ap = getLabelAddr(file.importedSyms, result[4], i.pc);
                        if(!ap.second) {
                            ap = getLabelAddr(file.exportedSyms, result[4], i.pc);
                        }
                        if(!ap.second) {
                            ap = getLabelAddr(file.localSyms, result[4], i.pc);
                        }

                        if(!ap.second) {
                            // create temp label
                            SymReference dummy(std::make_shared<Sym>(file.pathm, result[4]));
                            file.localSyms.insert(dummy);
                            file.localDeps.insert(std::pair<SymReference, pc_t>(dummy, i.pc));
                        }

                    }
                    
                    insts.empalce_back(i);
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
    };

    struct AddiF {
        bool operator()(const std::string &line, AssemblingState &state, std::size_t &lineNum, FileState & file){
            boost::smatch result;
            if(boost::regex_match(line, result, ass::regex::arithmImm) && result[1] == "ADDI") {

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