#include "tomasulo/inst_buffer_row.hpp"

using uint = unsigned int;

void inst_buffer_row::add_inst(uint16_t inst){
	this->instruction = inst;
}

void inst_buffer_row::record_issue_at(uint cycle){
	this->issue_at = cycle;
} 

void inst_buffer_row::record_start_execute_at(uint cycle){
	this->start_execute_at = cycle;
} 

void inst_buffer_row::record_end_execute_at(uint cycle){
	this->end_execute_at = cycle;
} 

void inst_buffer_row::record_write_at(uint cycle){
	this->write_at = cycle;
} 

void inst_buffer_row::record_commit_at(uint cycle){
	this->commit_at = cycle;
}