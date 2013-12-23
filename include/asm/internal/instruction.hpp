#pragma once
#include "asm/internal/typedefs.hpp"
namespace ass { namespace internal {


enum class InstType {LW, SW, ADDI, JMP, LI, JALR, JALI, ADD, SUB, MUL,
                    DIV, HALT, AND, OR, XOR, NOT, RET, BEQ, BNE, BGT, BGE, BLT, BLE};

struct Instruction {
    pc_t pc;
    addr_t data;
    InstType type;    
};

}}