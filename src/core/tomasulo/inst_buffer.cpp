#include "tomasulo/inst_buffer.hpp"
#include <iostream>

using uint = unsigned int;

inst_buffer::inst_buffer(uint size)
	:number_of_entries(size){}

//insert new entry in the buffer 
//returns true if inserted at last otherwise false
bool inst_buffer::insert_instruction(uint16_t inst){
	//std::cout << entries.size() << ","<< number_of_entries<<"\n" ;
	for(unsigned int i =0 ; i < number_of_entries ;i++){
		if(!entries.empty()){
			std::cout << i << ":";
			entries[i].print_row_contents();//msh sha3'ala
		}
		std::cout << "\n";
	}
	if(entries.size() > number_of_entries){
		return false;
	}
	else{
		inst_buffer_row new_row;
		new_row.add_inst(inst);
		new_row.record_issue_at(0);
		new_row.record_start_execute_at(0);
		new_row.record_end_execute_at(0);
		new_row.record_write_at(0);
		new_row.record_commit_at(0);
		return true;
	}
}

unsigned int inst_buffer::size(){
	return number_of_entries;
}