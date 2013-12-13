#ifndef main_memory_H
#define main_memory_H
#include "i_memory.h"

//size = 64 * 1024 * 8 /16 for 64KB and 16-bit words
#define MAIN_MEMORY_SIZE 32768

class main_memory : public i_memory
{
public:
    // initializers
    main_memory(unsigned int delay_in_cycles);

    //member functions
    virtual unsigned short get_data(unsigned short address, int &delay);
    virtual void write_data(unsigned short address, unsigned short data, int &delay);
    virtual memory_block fetch_block(unsigned short address, int &delay);
    virtual void write_block(memory_block block, int &delay);

    //member variables
private:
    unsigned int main_memory_size;
    unsigned int line_size;
    unsigned int associativity;
    unsigned int delay_in_cycles;
};

#endif // main_memory_H
