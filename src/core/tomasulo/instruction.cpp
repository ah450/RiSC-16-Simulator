#include "../../../include/tomasulo/instruction.hpp"
#include <cstdint>
#include <cmath>

using ushort = std::uint16_t;

OP Instruction::get_type(){
    ushort opcode = binary>>9 ;
    return map(opcode);
}

std::uint8_t Instruction::get_operand(const uint8_t index){//index 0 , 1 , 2
    ushort mask = 7 << (3 * index);
    return (binary & mask) >> (3 * index);
}

std::uint16_t Instruction::get_immediate(){
    return binary & 511;
}

Op map(ushort opcode){
    switch (opcode) {
    case 0xe0:
        return OP::ADD;
        break;
    case 0xe2:
        return OP::SUB;
        break;
    case 0xe4:
        return OP::MUL;
        break;
    case 0xe6:
        return OP::DIV;
        break;
    case 0xe8:
        return OP::HALT;
        break;
    case 0xea:
        return OP::AND;
        break;
    case 0xec:
        return OP::OR;
        break;
    case 0xee:
        return OP::XOR;
        break;
    case 0x00:
        return OP::LW;
        break;
    case 0x02:
        return OP::SW;
        break;
    default: return OP::HALT;
        break;
    }
}
