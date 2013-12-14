#include "cache.h"
#include <cmath>
#include <vector>
#include <iostream>

#define error(x) {std::cout<<"ERROR: "<<x<<std::endl; return false;}
#define MINIMU_CACHE_SIZE 2
#define MAXIMUM_CACHE_SIZE 65536
#define ADDRESS_SIZE 16
#define mask(x) (pow(2,x)-1) //mask bits

using ushort = unsigned short;

cache::cache(unsigned int cache_size,
             unsigned int line_size,
             unsigned int associativity)
    :cache_size(cache_size),line_size(line_size)
    ,associativity(associativity), running(false){}

void cache::on_cache_hit(HIT_POLICY write_hit){
    if(!running)
        this->write_hit = write_hit;
}

void cache::on_cache_miss(MISS_POLICY write_miss){
    if(!running)
        this->write_miss = write_miss;
}

void cache::cache_delay(int delay_in_cycles){
    if(!running)
        this->delay_in_cycles = delay_in_cycles;
}

bool cache::run(){

    if(!running )
        if(validate()){
            number_of_lines = cache_size / line_size;
            // initialize multidimensional vector
            //warning : should maintain fixed size
            memory_array = std::vector< std::vector<ushort> >
                    (number_of_lines, std::vector<ushort>
                     ((line_size/2)+2,0));// extra two fields for tag
            //, valid and dirty bits

            //set index, tag, offset
            offset_bits = ceil(log2(line_size));
            index_bits = ceil(log2(number_of_lines/associativity));
            tag_bits = ADDRESS_SIZE - (offset_bits + index_bits);

            this->running = true;
            return true;
        }
    return false;
}

bool cache::validate(){
    std::cout<< "***validating***" <<std::endl;
    if (cache_size < MINIMU_CACHE_SIZE )
        error("small cache size is too small");
    if (cache_size > MAXIMUM_CACHE_SIZE)
        error("cache size is too big");
    if (log2(cache_size) != floor(log2(cache_size)) )
        error("cache size is not multiple of 2");

    if (line_size < MINIMU_CACHE_SIZE )
        error("line size is too small");
    if (cache_size > MAXIMUM_CACHE_SIZE)
        error("line size is too big");
    if ((cache_size % 2) == 1)
        error("line size is not even");

    unsigned int max_associativity = cache_size / line_size;
    if (associativity < 1 )
        error("associativity is too small");
    if (associativity > max_associativity)
        error("associativity is too big");

    if(delay_in_cycles <1)
        error("delay in cycles is too small");
    return true;
}

ushort cache::get_data(ushort address, int &delay){

    //get index, tag & offset
    ushort offset = get_offset(address);
    ushort index = get_index(address);
    ushort tag = get_tag(address);
    //direct mapped
    std::vector<ushort>  line= memory_array[index];
    ushort line_tag = line[(line_size/2)];
    if (tag == line_tag) //cache hit
        return line[offset/2];
    return 0;
}

void cache::write_data(unsigned short address,
                       unsigned short data, int &delay){

}

memory_block cache::fetch_block(unsigned short address, int &delay){
    memory_block block;
    return block;
}

void cache::write_block(memory_block block, int &delay){

}

unsigned int cache::get_hit_ratio(){
    return 0;
}

ushort cache::get_offset(ushort address){
    ushort offset = address & (ushort)mask(offset_bits) ;
    return offset;
}

ushort cache::get_index(ushort address){
    ushort index = address & ((ushort)mask(index_bits)
                                      << offset_bits);
    index = index >> offset_bits;
    return index;
}

ushort cache::get_tag(ushort address){
    ushort tag = address & ((ushort)mask(tag_bits)
                                      << (offset_bits + index_bits));
    tag = tag >> (offset_bits + index_bits);
    return tag;
}
