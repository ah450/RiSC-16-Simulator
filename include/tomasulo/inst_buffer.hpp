#ifndef INST_BUFFER_H
#define INST_BUFFER_H
#include "inst_buffer_row.hpp"
#include <list>
#include <stdint.h>

class inst_buffer{	
private:
	unsigned int number_of_entries;
    std::list <inst_buffer_row> entries;

public:
    inst_buffer(unsigned int size);
    set_i_cache(std::shared_ptr<i_memory> instruction_cache);
    virtual void tick (tomasulo &t);//handles fetching and issuing
};

#endif
