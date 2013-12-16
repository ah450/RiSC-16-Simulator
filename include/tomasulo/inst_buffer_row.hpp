#ifndef INST_BUFFER_ROW_H
#define INST_BUFFER_ROW_H

class inst_buffer_row{
private:
	std::uint16_t instruction;
	unsigned int issue_at;
	unsigned int start_execute_at;
	unsigned int end_execute_at;
	
};

#endif