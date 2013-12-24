#include "tomasulo/instruction.hpp"

OP Instruction::get_type(){
    uint16_t offset4 = (binary >> 9) & 15;
    uint16_t offset3= (binary >> 13) & 7;
    OP op;
    if(offset3 != 7){
        switch(offset3){
            case 0:
                op = OP::LW;
                break;
            case 1:
                op = OP::SW;
                break;
            case 2:
                op = OP::ANDI;
                break;
            case 3:
                op = OP::JMP;
                break;
            case 4:
                op = OP::LI;
                break;
            case 5:
                op = OP::JALR;
                break;
            case 6:
                op = OP::JALI;
                break;
        }
    }
    else{
        switch(offset4){
            case 0:
                op = OP::ADD;
                break;
            case 1:
                op = OP::SUB;
                break;
            case 2:
                op = OP::MUL;
                break;
            case 3:
                op = OP::DIV;
                break;
            case 4:
                op = OP::HALT;
                break;
            case 5:
                op = OP::AND;
                break;
            case 6:
                op = OP::OR;
                break;
            case 7:
                op = OP::XOR;
                break;
            case 8:
                op = OP::NOT;
                break;
            case 9:
                op = OP::RET;
                break;
            case 10:
                op = OP::BEQ;
                break;
            case 11:
                op = OP::BNE;
                break;
            case 12:
                op = OP::BGT;
                break;
            case 13:
                op = OP::BLT;
                break;
            case 14:
                op = OP::BGE;
                break;
            case 15:
                op = OP::BLE;
                break;
        }
    }
    return op;
}

std::uint8_t Instruction::get_operand(const uint8_t index){
    if(index > 3 || index < 1){
        InstructionException("Invalid operand index is requested");
    }
    uint16_t offset4 = (binary >> 9) & 15;
    uint16_t offset3 = (binary >> 13) & 7;
    uint8_t operand1 = (binary >> 10) & 7;
    uint8_t operand2 = (binary >> 7) & 7;
    uint8_t operand3;
    if(offset3 != 7){
        switch(offset3){
            case 0:
            case 1:
            case 2:
            case 5:
                if(index > 2){
                    InstructionException("LW, SW and ANDI, JALR instructions have only two operands");
                }
                break;
            case 3:
            case 4:
            case 6:
                if(index > 1){
                    InstructionException("JMP, LI and JALI instructions have only one operand");
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
    uint16_t offset3= (binary >> 13) & 7;
    uint16_t result;
    if(offset3 != 7 && offset3 != 5){//offset3 !=3 and not a JALR instruction
        result = binary & 127;
        if(offset3 == 3 || offset3 == 4 || offset3 == 6){
            result = binary & 1023;
            //sign extend
            if(result & 512 != 0){
                result |= 0xFC00;
            }
        }
        else{
            result = binary & 127;
            //sign extend
            if(result & 64 != 0){
                result |= 0xFF80;
            }
        }
    }
    else{
        InstructionException("Instruction requested has no immediate value");
    }
    return result;
}

enum class OP {LW, SW, ADDI, JMP, LI, JALR, JALI, ADD, SUB,
    MUL, DIV, HALT, AND, OR, XOR, NOT, RET, BEQ, BNE, BGT,
    BLT, BGE, BLE};


std::uint16_t Instruction::get_branch_addr(tomasulo & t){
    OP t = get_type();
    if(t == OP::JMP || t == OP::JALR || t == OP::JALI || 
        t == OP::RET || t == OP::BEQ || t == OP::BNE || 
        t == OP::BGT || t == OP::BLT || t == OP::BGE || 
        t == OP::BLE ){
    }
    std::uint16_t pc = t.PC;
    std::uint16_t imm;
    std::uint8_t reg;
    std::uint16_t reg_content;
    std::uint16_t predicted_addr;
    switch(t){
        case OP::JMP:
            imm = get_immediate();
            reg = get_operand(1);
            if(reg == 0){
                InstructionException("REGS[0] cannot be accessed");        
            }
            reg_content = t.regFile.regs[reg];
            predicted_addr = pc + reg_content + imm + 1;
            break;
        case JALI:
            imm = get_immediate();
            predicted_addr = pc + imm + 1;
            break;
        case JALR:
            reg = get_operand(2);
            if(reg == 0){
                InstructionException("REGS[0] cannot be accessed");        
            }
            reg_content = t.regFile.regs[reg];
            predicted_addr = reg_content + pc + 1;
            break;
        case RET:
            reg = get_operand(1);
            if(reg == 0){
                InstructionException("REGS[0] cannot be accessed");        
            }
            reg_content = t.regFile.regs[reg];
            predicted_addr = reg_content;
            break;
        case OP::BEQ:
        case OP::BNE:
        case OP::BGT:
        case OP::BLT:
        case OP::BGE:
        case OP::BLE:
            reg = get_operand(3);
            if(reg == 0){
                InstructionException("REGS[0] cannot be accessed");        
            }
            reg_content = t.regFile.regs[reg];
            predicted_addr = reg_content;
            break;
        default:
            InstructionException("Error while computing branch address");
            break;
    }
    return predicted_addr;
}
