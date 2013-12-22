#ifndef INST_BUFFER_H
#define INST_BUFFER_H
#include "inst_buffer_row.hpp"
#include <queue>
#include <stdint.h>

class inst_buffer{	
private:
	unsigned int number_of_entries;
    std::queue <inst_buffer_row> entries;

public:
    inst_buffer(unsigned int size);
	virtual bool insert_instruction(uint16_t instruction);
	virtual unsigned int size();

    //add a function to change the register board when issue
};

#endif
