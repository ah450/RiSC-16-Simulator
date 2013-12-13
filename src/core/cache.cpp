#include "cache.h"
#include <vector>

#define error(x) std::cout<<"ERROR: "<<x<<std::endl; return false;
#define MINIMU_CACHE_SIZE 2
#define MAXIMUM_CACHE_SIZE 65536

cache::cache(unsigned int cache_size,
             unsigned int line_size,
             unsigned int associativity)
    :cache_size(cache_size),line_size(line_size)
    ,associativity(associativity), running(false){}

cache::on_cache_hit(HIT_POLICY write_hit){
    if(!running)
        this->write_hit = write_hit;
}

cache::on_cache_miss(MISS_POLICY write_miss){
    if(!running)
        this->write_miss = write_miss;
}

cache::cache_delay(int delay_in_cycles){
    if(!running)
        this->delay_in_cycles = delay_in_cycles;
}

cache::run(){
    if(!running)
        if(validate()){
            number_of_lines = cache_size / line_size;
            // initialize multidimensional vector
            //warning : should maintain fixed size
            memory_array = std::vector< std::vector<unsigned short> >
                    (number_of_lines, std::vector<unsigned short>
                     (line_size,0));
            this->running = true;
        }
}

cache::validate(){
    if (cache_size < MINIMU_CACHE_SIZE )
        error("small cache size is too small");
    if (cache_size > MAXIMUM_CACHE_SIZE)
        error("cache size is too big");

    if (line_size < MINIMU_CACHE_SIZE )
        error("line size is too small");
    if (cache_size > MAXIMUM_CACHE_SIZE)
        error("line size is too big");

    unsigned int max_associativity = cache_size / line_size;
    if (associativity < 1 )
        error("associativity is too small");
    if (cache_size > max_associativity)
        error("associativity is too big");

    return true;
}
