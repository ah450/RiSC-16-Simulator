#ifndef INST_BUFFER_ROW_H
#define INST_BUFFER_ROW_H
#include <stdint.h>
#include "instruction.hpp"

class inst_buffer_row{
private:
    Instruction instruction;
    bool empty;
public:
    virtual void add_inst(uint16_t instruction);
    virtual Instruction pop_inst();
};

#endif
