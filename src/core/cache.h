#ifndef cache_H
#define cache_H
#include "i_memory.h"
#include <memory>
#include <vector>
#include <iostream>

class cache : public i_memory
{
public:
    // initializers
    cache(unsigned int cache_size,unsigned int line_size,unsigned int associativity);
    void on_cache_hit(HIT_POLICY write_hit);
    void on_cache_miss(MISS_POLICY write_miss);
    void cache_delay(int delay_in_cycles);
    bool run();

    //member functions
    virtual unsigned short get_data(unsigned short address, int &delay);
    virtual void write_data(unsigned short address, unsigned short data, int &delay);
    virtual memory_block fetch_block(unsigned short address, int &delay);
    virtual bool write_block(memory_block block, int &delay);
    virtual unsigned int get_hit_ratio();

    //member variables
private:
    unsigned int cache_size;// in bytes
    unsigned int line_size;// in bytes
    unsigned int associativity;
    unsigned int delay_in_cycles;
    unsigned int cache_accesses;
    unsigned int cache_misses;
    unsigned int number_of_lines;
    unsigned int index_bits;
    unsigned int tag_bits;
    unsigned int offset_bits;
    bool running;
    std::vector< std::vector<unsigned short> > memory_array;
    HIT_POLICY write_hit;
    MISS_POLICY write_miss;
    std::weak_ptr<i_memory> lower_memory;

    //internal functions
    bool validate();
    unsigned short get_replacement(unsigned short address);
    bool hit_or_miss(ushort tag, ushort &index);
    void read_address (unsigned short address, unsigned short &offset,
                       unsigned short &index, unsigned short &tag);
};

#endif // cache_H
