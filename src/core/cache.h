#ifndef cache_H
#define cache_H
#include "i_memory.h"

class cache : public i_memory
{
public:
    // initializers
    cache(unsigned int cache_size,unsigned int line_size,unsigned int associativity);
    void on_cache_hit(HIT_POLICY write_hit);
    void on_cache_miss(MISS_POLICY write_miss);
    void cache_delay(int delay_in_cycles);

    //member functions
    virtual unsigned short get_data(unsigned short address, int &delay);
    virtual void write_data(unsigned short address, unsigned short data, int &delay);
    virtual memory_block fetch_block(unsigned short address, int &delay);
    virtual void write_block(memory_block block, int &delay);
    virtual unsigned int get_hit_ratio();

    //member variables
private:
    unsigned int cache_size;
    unsigned int line_size;
    unsigned int associativity;
    unsigned int delay_in_cycles;
    unsigned int cache_accesses;
    unsigned int cache_misses;
    HIT_POLICY write_hit;
    MISS_POLICY write_miss;
    i_memory lower_memory;
};

#endif // cache_H
