#ifndef cache_H
#define cache_H
#include "i_memory.h"

class cache : public i_memory
{
public:
    // initializers
    cache();
    cache(int cache_size, int line_size, int associativity);
    void on_cache_hit(HIT_POLICY write_hit);
    void on_cache_miss(MISS_POLICY write_miss);
    void cache_delay(int delay_in_cycles);

    //member functions
    virtual unsigned short get_data(unsigned short address);
    virtual void write_data(unsigned short address, unsigned short data);
    virtual bool write_block(memory_block data);

    //member variables
private:
    int cache_size;
    int line_size;
    int associativity;
    int delay_in_cycles;
    HIT_POLICY write_hit;
    MISS_POLICY write_miss;
};

#endif // cache_H
