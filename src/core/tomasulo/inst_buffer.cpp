#include "tomasulo/inst_buffer.hpp"
#include <iostream>

using uint = unsigned int;

inst_buffer::inst_buffer(uint size)
	:number_of_entries(size){}

bool inst_buffer::insert_instruction(uint16_t inst){
	if(entries.size() == number_of_entries){
		return false;
	}
	else{
        entries.push_back(inst);//change this
		return true;
	}
}

unsigned int inst_buffer::size(){
	return number_of_entries;
}
