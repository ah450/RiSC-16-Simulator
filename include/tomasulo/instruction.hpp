#ifndef INSTRUCTION_H
#define INSTRUCTION_H

enum class OP {LW, SW, ADDI, JMP, LI, JALR, JALI, ADD, SUB,
    MUL, DIV, HALT, AND, OR, XOR, NOT, RET, BEQ, BNE, BGT,
    BLT, BGE, BLE};

struct Instruction{
    std::uint16_t binary;
    OP get_type();
    std::uint8_t get_operand(const uint8_t index);
    std::uint16_t get_immediate(); //implement exceptions for non immediate inestructions
};

#endif // INSTRUCTION_H
