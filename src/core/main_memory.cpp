#include "main_memory.h"
#include <cstdint>

#define MEMORY_SIZE 65536

using uint = uint32_t;
using ushort = uint16_t;

memory_block main_memory::fetch_block(ushort address, int size, int &delay){
    memory_block block;
    block.data = std::vector<ushort>(1,0);
    copy(memory.begin()+address/2,memory.begin()+address/2+size,block.data.begin());
    block.start_address = address ;
    return block;
}

bool main_memory::write_block(memory_block block, int &delay){
    copy(block.data.begin(),block.data.end(),memory.begin()+block.start_address/2);
    return true;
}

ushort main_memory::get_data(ushort address, int &delay){
    delay += delay_in_cycles;
    return memory[address/2];
}

void main_memory::write_data(ushort address, ushort data, int &delay){
    delay += delay_in_cycles;
    memory[address/2] = data;
}

uint main_memory::get_delay(){
    return delay_in_cycles;
}

bool main_memory::run(){
    memory = std::vector< ushort >(MEMORY_SIZE/2,(ushort)0);
    return true;
}
