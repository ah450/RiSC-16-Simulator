#ifndef INST_BUFFER_H
#define INST_BUFFER_H
#include "inst_buffer_row.hpp"
#include <deque>
#include <stdint.h>

class inst_buffer{	
private:
	unsigned int number_of_entries;
	std::deque<uint16_t> entries;

public:
	inst_buffer(unsigned int size);
	virtual bool insert_instruction(uint16_t instruction);
	virtual unsigned int size();
};

#endif