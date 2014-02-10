#ifndef INSTRUCTION_H
#define INSTRUCTION_H
#include "core/typedefs.hpp"
#include "core/exceptions.hpp"
#include <iostream>




struct Instruction {
    std::uint16_t binary;
    address_t pc;
    OP type;
    Instruction( std::uint16_t binary, address_t pc );
    std::uint8_t get_operand( const uint8_t index ) const;
    data_t get_immediate()
    const; //implement exceptions for non immediate instructions
    address_t get_branch_addr() const;//returns the predicted address
};


inline std::ostream &operator<<( std::ostream &out, const Instruction &i ) {
    out << "PC : " << i.pc <<
        "\n DATA: " << std::hex << i.binary
        << std::dec << "\nType: ";

    switch ( i.type ) {
        case OP::LW:
            out << "LW\n";
            break;

        case OP::SW:
            out << "SW\n";
            break;

        case OP::ADDI:
            out << "ADDI\n";
            break;

        case OP::JMP:
            out << "JMP\n";
            break;

        case OP::LI:
            out << "LI\n";
            break;

        case OP::JALR:
            out << "JALR\n";
            break;

        case OP::JALI:
            out << "JALI\n";
            break;

        case OP::ADD:
            out << "ADD\n";
            break;

        case OP::SUB:
            out << "SUB\n";
            break;

        case OP::MUL:
            out << "DIV\n";
            break;

        case OP::HALT:
            out << "HALT\n";
            break;

        case OP::AND:
            out << "AND\n";
            break;

        case OP::OR:
            out << "OR\n";
            break;

        case OP::XOR:
            out << "XOR\n";
            break;

        case OP::NOT:
            out << "NOT\n";
            break;

        case OP::RET:
            out << "RET\n";
            break;

        case OP::BEQ:
            out << "BEQ\n";
            break;

        case OP::DIV:
            out << "DIV\n";

    }

    return out;
}

#endif // INSTRUCTION_H
