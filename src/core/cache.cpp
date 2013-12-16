#include "cache.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>

#define error(x) {std::cout<<"ERROR: "<<x<<std::endl; return false;}
#define MINIMU_CACHE_SIZE 2
#define MAXIMUM_CACHE_SIZE 65536
#define ADDRESS_SIZE 16
#define mask(x) (pow(2,x)-1) //mask bits

using ushort = unsigned short;
using uint = unsigned int;

cache::cache(uint cache_size,
             uint line_size,
             uint associativity)
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

    uint max_associativity = cache_size / line_size;
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
    ushort offset , index, tag;
    read_address(address, offset,  index,  tag);
    bool found = hit_or_miss(tag, index);
    if(found){//read hit
        std::cout<<"INFO: "<<"read hit"<<std::endl;
        delay = delay_in_cycles;
        return(memory_array[index][offset/2]);
     }else
        error("read miss");
}

void cache::write_data(unsigned short address,
                       unsigned short data, int &delay){
    ushort offset , index, tag;
    read_address(address, offset,  index,  tag);
    bool found = hit_or_miss(tag, index);
    if(found){//write hit
        std::cout<<"INFO: "<<"write hit"<<std::endl;
        memory_array[index][offset/2] = data;
        if(write_hit == HIT_POLICY::WRITE_THROUGH){
            /* write to main memory*/
        }
    }else{//write miss
        std::cout<<"ERROR: "<<"write miss"<<std::endl;
        if(write_miss == MISS_POLICY::WRITE_ALLOCATE){
            /* fetch block then write */
        }else{
            /* update main memory only */
        }
    }
}

//not implemented
memory_block cache::fetch_block(unsigned short address, int &delay){
    memory_block block;
    return block;
}


//where the block size is the same as the line size
//warning dirty block not considered
bool cache::write_block(memory_block block, int &delay){
    ushort offset , index, tag;
    read_address(block.start_address, offset, index, tag);
    uint size = block.data.size();
    if(size != line_size/2)
        error("block not equal to line_size");
    index = get_replacement(block.start_address);
    std::cout<<index<<std::endl;
    //handle dirty here
    if(memory_array[index][line_size/2+1] & (ushort) 0x2)//block dirty
    {/*pass block to lower level*/}
    memory_array[index][line_size/2] = tag;//set tag
    memory_array[index][line_size/2+1] = (ushort) 0x1; //set valid bit
    //copy data
    copy(block.data.begin(),block.data.end(),memory_array[index].begin());
    return true;
}

//not implemented
uint cache::get_hit_ratio(){
    return 0;
}

ushort cache::get_replacement(ushort address){//using Random replacement
    //check valid bits
    ushort offset , index, tag;
    read_address(address, offset, index, tag);
    for (int i = 0; i < (short)associativity; ++i) {
        if(!(memory_array[index][line_size/2+1] & (ushort) 0x1))
            return index;
        index+= number_of_lines/associativity;
    }//all are valid choose a random element
    index-= number_of_lines/associativity;
    std::srand(time(NULL));
    int ind = std::rand() % associativity;
    return index + ind *(number_of_lines / associativity);
}

bool cache::hit_or_miss(ushort tag, ushort &index){
    for (int i = 0; i < (short)associativity; ++i) {
        if((memory_array[index][line_size/2+1] && (ushort) 0x1))//index is valid
            if(memory_array[index][line_size/2] == tag){//the same tag
                return true;
            }
        index+= number_of_lines/associativity;
    }
    return false;
}

void cache::read_address(ushort address, ushort &offset, ushort &index, ushort &tag){
    offset = address & (ushort)mask(offset_bits) ;
    index = address & ((ushort)mask(index_bits)
                                          << offset_bits);
    index = index >> offset_bits;
    tag = address & ((ushort)mask(tag_bits)
                                          << (offset_bits + index_bits));
    tag = tag >> (offset_bits + index_bits);
    return;
}
