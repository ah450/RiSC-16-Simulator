#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include <stdexcept>

// 23 operations for 23 different types of instructions
enum class OP {LW, SW, ADDI, JMP, LI, JALR, JALI, ADD, SUB,
    MUL, DIV, HALT, AND, OR, XOR, NOT, RET, BEQ, BNE, BGT,
    BLT, BGE, BLE};

struct InstructionException : std::runtime_error {
        InstructionException(const std::string &what): std::runtime_error(what){}
        InstructionException(const InstructionException &) = default;
        InstructionException(InstructionException &&) = default;
};


struct Instruction{
    std::uint16_t binary;
    OP get_type();
    std::uint8_t get_operand(const uint8_t index);
    std::uint16_t get_immediate(); //implement exceptions for non immediate instructions
    std::uint16_t get_branch_addr(tomasulo & t);//returns the predicted address
};


#endif // INSTRUCTION_H
