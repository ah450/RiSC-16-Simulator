#include "tomasulo/inst_buffer_row.hpp"
#include <iostream>

using uint = unsigned int;

void inst_buffer_row::add_inst(uint16_t inst){
	instruction = inst;
}

void inst_buffer_row::record_issue_at(uint cycle){
	issue_at = cycle;
} 

void inst_buffer_row::record_start_execute_at(uint cycle){
	start_execute_at = cycle;
} 

void inst_buffer_row::record_end_execute_at(uint cycle){
	end_execute_at = cycle;
} 

void inst_buffer_row::record_write_at(uint cycle){
	write_at = cycle;
} 

void inst_buffer_row::record_commit_at(uint cycle){
	commit_at = cycle;
}

void inst_buffer_row::print_row_contents(){
	std::cout << instruction << "," << issue_at << "," 
		<< start_execute_at << "," << end_execute_at << 
		write_at << "," << commit_at;
}