#pragma once
#include "asm/internal/typedefs.hpp"
#include <iostream>

namespace ass {
namespace internal {


enum class InstType {
    LW, SW, ADDI, JMP, LI, JALR, JALI, ADD, SUB, MUL,
    DIV, HALT, AND, OR, XOR, NOT, RET, BEQ
};

struct Instruction {
    pc_t pc;
    addr_t data;
    InstType type;

};



}
}