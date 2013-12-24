#pragma once 
#include "asm/internal/internal.hpp"
#include <utility>

namespace ass { namespace internal {
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




inline void fillInstruction(Instruction &i, addr_t value) {
    switch(i.type){
        case InstType::LW :
        case InstType::SW :
            i.data |= 0x7F & value;
            break;

    }
}

inline void resolveLabelHelper(SymReference &sr, AssemblineState &state, FileState &tu,
             const std::size_t lineNum, Instruction &i ) {

    if(sr.ref->defined()) {
        fillInstricion(i, sr.ref->value());
    }else {
        tu.localDeps.insert(std::pair<SymReference, pc_t>(sr, i.pc));
    }

}
/**
 * @brief Attempts to resolve a label reference.
 * @details Checks the imported , exported and local Syms
 * of the current translation unit. If not found in any.
 * A forward decleration is made in the local Syms set.
 * Aswell as a dependency in the localDeps map.
 * If found as a forward decleration. if The symbol is local
 * a local dependency is added. if it is global a global
 * a global dependency is added.
 * Note: It will not add a global dependency if it was not previously
 * imported.
 * Note2: instruction pc MUST be set 
 * 
 * @param name label name.
 * @param state global state.
 * @param  tu current translation unit state.
 * @param lineNum line number of use.
 * @param i instruction referencing the label.
 * @return true if succesfull.
 */
inline bool resolveLabel(const std::string & name, AssemblingState &state, FileState &tu,
                const std::size_t lineNum, Instruction & i){


    for (auto & ref : tu.importedSyms){
        if(ref.ref->name() == name) {
            resolveLabelHelper(ref, state, tu, lineNum, i );
            return true;
        }
    }

     for (auto & ref : tu.exportedSyms){
        if(ref.ref->name() == name) {
            resolveLabelHelper(ref, state, tu, lineNum, i );
            return true;
        }
    }

     for (auto & ref : tu.localSyms){
        if(ref.ref->name() == name) {
            resolveLabelHelper(ref, state, tu, lineNum, i );
            return true;
        }
    }

    // if execution reached this point then it hasn't been defined / forward declared before
    // non global forward declaration
    auto sr = SymReference(std::make_shared<Sym>(name, false));
    tu.localSyms.insert(sr);
    tu.localDeps.insert(std::pair<SymReference,pc_t>(sr, i.pc));
}

}}