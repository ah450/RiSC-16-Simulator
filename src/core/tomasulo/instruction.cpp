#include "../../include/tomasulo/instruction.hpp"

OP Instruction::get_type(){
    uint16_t offset4 = (binary >> 9) & 15;
    uint16_t offset3= (binary >> 13) & 7;
    OP op;
    if(offset3 != 7){
        switch(offset3){
            case 0:
                op = LW;
                break;
            case 1:
                op = SW;
                break;
            case 2:
                op = ANDI;
                break;
            case 3:
                op = JMP;
                break;
            case 4:
                op = LI;
                break;
            case 5:
                op = JALR;
                break;
            case 6:
                op = JALI;
                break;
        }
    }
    else{
        switch(offset4){
            case 0:
                op = ADD;
                break;
            case 1:
                op = SUB;
                break;
            case 2:
                op = MUL;
                break;
            case 3:
                op = DIV;
                break;
            case 4:
                op = HALT;
                break;
            case 5:
                op = AND;
                break;
            case 6:
                op = OR;
                break;
            case 7:
                op = XOR;
                break;
            case 8:
                op = NOT;
                break;
            case 9:
                op = RET;
                break;
            case 10:
                op = BEQ;
                break;
            case 11:
                op = BNE;
                break;
            case 12:
                op = BGT;
                break;
            case 13:
                op = BLT;
                break;
            case 14:
                op = BGE;
                break;
            case 15:
                op = BLE;
                break;
        }
    }
    return op;
}

std::uint8_t Instruction::get_operand(const uint8_t index){
    if(index > 3){
        InstructionException("Invalid operand index is requested");
    }
    uint16_t offset4 = (binary >> 9) & 15;
    uint16_t offset3= (binary >> 13) & 7;
    uint8_t operand1 = (binary >> 10) & 7;
    uint8_t operand2 = (binary >> 7) & 7;
    uint8_t operand3;
    if(offset3 != 7){
        switch(offset3){
            case 0:
            case 1:
            case 2:
                if(index > 2){
                    InstructionException("LW, SW and ANDI instructions have only two operand");
                }
                break;
            case 3:
            case 4:
            case 5:
            case 6:
                if(index > 1){
                    InstructionException("JMP, LI, JALR and JALI instructions have only one operand");
                }
                break;
        }
    }
    else{
        operand1 = (binary >> 6) & 7;
        operand2 = (binary >> 3) & 7;
        operand3 = binary & 3;
        switch(offset4){
            case 5:
                InstructionException("HALT instruction has no operands");
                break;
            case 9:
                if(index > 1){
                    InstructionException("RET instruction has only one operand");
                }
        }
    }
    if(index == 1){
        return operand1;
    }
    else if(index == 2){
        return operand2;
    }
    else{
        return operand3;
    }
}

std::uint16_t Instruction::get_immediate(){
    uint16_t offset4 = (binary >> 9) & 15;
    uint16_t offset3= (binary >> 13) & 7;
    uint16_t result;
    if(offset3 != 7){
        result = binary & 127;
        if(offset3 == 3 || offset3 == 4 || offset3 == 6){
            result = binary && 1023;
        }
    }
    else{
        InstructionException("Instruction requested has no immediate value");
    }
}



