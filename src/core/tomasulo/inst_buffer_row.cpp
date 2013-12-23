#include "../../../include/tomasulo/inst_buffer_row.hpp"
#include <iostream>
#include <cstdint>
#include "../../../include/tomasulo/instruction.hpp"
using uint = std::uint32_t;

void inst_buffer_row::add_inst(uint16_t inst){
    instruction = Instruction {inst};
    empty = false;
}

Instruction inst_buffer_row::pop_inst(){
    return Instruction;
    empty = true;
}
