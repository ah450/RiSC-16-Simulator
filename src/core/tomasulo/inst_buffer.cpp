#include "tomasulo/inst_buffer.hpp"

using uint = unsigned int;

inst_buffer::inst_buffer(uint size)
	:number_of_entries(size){}

//insert new entry in the buffer 
//returns true if inserted at last otherwise false
bool inst_buffer::insert_instruction(uint16_t inst){
	if(entries.size() > number_of_entries)
		return false;

	inst_buffer_row new_row;
	new_row.add_inst(inst);
	new_row.record_issue_at(0);
	new_row.record_start_execute_at(0);
	new_row.record_end_execute_at(0);
	new_row.record_write_at(0);
	new_row.record_commit_at(0);
	return true;
}

unsigned int inst_buffer::size(){
	return number_of_entries;
}