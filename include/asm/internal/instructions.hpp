#pragma once
#include <unordered_map>
#include <memory>
namespace ass { namespace inst {
enum class InstType {LW, SW, ADDI, JMP, LI, JALR, JALI, ADD, SUB, MUL,
                    DIV, NAND, AND, OR, XOR, NOT, RET, BEQ, BNE, BGT, BGE, BLT, BLE};


struct Instruction {

    virtual ~Instruction();

};



}}