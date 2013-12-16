#ifndef INST_BUFFER_H
#define INST_BUFFER_H
#include "inst_buffer_row.hpp"
#include <vector>

class inst_buffer : public inst_buffer_row{	
private:
	unsigned int number_of_enteries;
	std::vector<inst_buffer_row> enteries;

};

#endif