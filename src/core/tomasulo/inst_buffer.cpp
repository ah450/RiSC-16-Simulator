#include "../../../include/tomasulo/inst_buffer.hpp"
#include "../../../include/tomasulo/instruction.hpp"
#include "../../../include/tomasulo/inst_buffer_row.hpp"
#include <cstdint>
#include <memory>
#include <list>

using uint = unsigned int;

inst_buffer::inst_buffer(uint size)
    :number_of_entries(size){
    entries = std::list();
    current_address = 0;
}

void inst_buffer::set_i_cache
    (std::shared_ptr<i_memory> instruction_cache){
    this->instruction_cache = instruction_cache;
}

void inst_buffer::flush(uint16_t address){
    entries.popAll();
    current_address = address;
}

void inst_buffer::tick(tomasulo &t){
    //fetch new instruction
    //delay here ?
    if(entries.size() < number_of_entries){
        entries.push_back(inst_buffer_row{
            instruction_cache.get_data(current_address)
        }
            );
        current_address += 2;
    }
    //if possiple issue first instruction
    inst_buffer_row
    bool result = entries.front().issue(t);
    if(result){
        Instruction inst = entries.pop_front().instruction;
        OP op = instruction.get_type();
        if (op == OP::BGT || op == OP::BLT || op == OP::BGE
            op == OP::BGE || op == OP::BEQ || op == OP::BNE)
        {//branch assume taken
            current_address += inst.get_immdediate()-2;
        }

    }
}

