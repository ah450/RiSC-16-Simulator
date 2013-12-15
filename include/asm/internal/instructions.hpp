#pragma once
#include <unordered_map>
#include <cstding>
namespace ass { namespace internal {
enum class InstType {LW, SW, ADDI, JMP, LI, JALR, JALI, ADD, SUB, MUL,
                    DIV, HALT, AND, OR, XOR, NOT, RET, BEQ, BNE, BGT, BGE, BLT, BLE};


struct Instruction {

    std::uint16_t data;
    InstType type;
    std::uint16_t pc;
};



}}