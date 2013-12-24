#ifndef INST_BUFFER_H
#define INST_BUFFER_H
#include "inst_buffer_row.hpp"
#include <list>
#include <cstdint>
#include <memory>

class inst_buffer{	
private:
	unsigned int number_of_entries;
    std::list <inst_buffer_row> entries;
    std::shared_ptr<i_memory> instruction_cache;
    uint16_t current_address ;

public:
    inst_buffer(unsigned int size);
    virtual void flush(uint16_t address);
    virtual void set_i_cache(std::shared_ptr<i_memory> instruction_cache);
    virtual void tick (tomasulo &t);//handles fetching and issuing
};

#endif
