#include "../../../include/tomasulo/inst_buffer_row.hpp"
#include <iostream>
#include <cstdint>
#include "../../../include/tomasulo/instruction.hpp"
using uint = std::uint32_t;

void inst_buffer_row::inst_buffer_row(uint16_t inst){
    instruction = Instruction {inst};
}

bool inst_buffer::issue(tomasulo &t){
	OP = instruction.get_type();
 	if (/* empty reservation station & !rob.is_full() */){
		/* fill rob then reservation*/
		return true;
 	   }   
 	   	return false;
}