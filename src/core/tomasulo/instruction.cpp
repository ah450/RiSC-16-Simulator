#include "core/typedefs.hpp"
#include "core/instruction.hpp"
#include "core/tomasulo.hpp"


Instruction::Instruction( std::uint16_t binary,
                          address_t pc ) : binary( binary ), pc( pc ) {
    uint16_t offset4 = ( binary >> 9 ) & 15;
    uint16_t offset3 = ( binary >> 13 ) & 7;

    if ( offset3 != 7 ) {
        switch ( offset3 ) {
            case 0:
                type = OP::LW;
                break;

            case 1:
                type = OP::SW;
                break;

            case 2:
                type = OP::ADDI;
                break;

            case 3:
                type = OP::JMP;
                break;

            case 4:
                type = OP::BEQ;
                break;

            case 5:
                type = OP::JALR;
                break;

            case 6:
                type = OP::JALI;
                break;
        }
    } else {
        switch ( offset4 ) {
            case 0:
                type = OP::ADD;
                break;

            case 1:
                type = OP::SUB;
                break;

            case 2:
                type = OP::MUL;
                break;

            case 3:
                type = OP::DIV;
                break;

            case 4:
                type = OP::HALT;
                break;

            case 5:
                type = OP::AND;
                break;

            case 6:
                type = OP::OR;
                break;

            case 7:
                type = OP::XOR;
                break;

            case 8:
                type = OP::NOT;
                break;

            case 9:
                type = OP::RET;
                break;
        }

        if ( ( binary & 0xFE00 ) == 0xF400 ) {
            type = OP::LI;
        }
    }


}

std::uint8_t Instruction::get_operand( const uint8_t index ) const {
    uint8_t operand1;
    uint8_t operand2;
    uint8_t operand3;

    switch ( type ) {
        case OP::LW:
        case OP::SW:
            operand1 = ( binary & 0x1C00 ) >> 10;
            operand2 = ( binary & 0x0380 ) >> 7;

            if ( index == 3 ) {
                throw InstructionException( "SW , LW instructions has no 3rd operand" );
            }

            break;

        case OP::ADDI:
            operand1 = ( binary & 0x1C00 ) >> 10;
            operand2 = ( binary & 0x0380 ) >> 7;

            if ( index == 3 ) {
                throw InstructionException( "ADDI instruction has no 3rd operand" );
            }

            break;

        case OP::BEQ:
            operand1 = ( binary & 0x1C00 ) >> 10;
            operand2 = ( binary & 0x0380 ) >> 7;

            if ( index == 3 ) {
                throw InstructionException( "BEQ instruction has no 3rd operand" );
            }

            break;

        case OP::JALR:
            operand1 = ( binary & 0x1C00 ) >> 10;
            operand2 = ( binary & 0x0380 ) >> 7;

            if ( index == 3 ) {
                throw InstructionException( "JALR instruction has no 3rd operand" );
            }

        case OP::JMP:
            operand1 = ( binary & 0x1C00 ) >> 10;

            if ( index != 1 ) {
                throw InstructionException( "JMP instruction has only one operand" );
            }

            break;

        case OP::JALI:
            operand1 = ( binary & 0x1C00 ) >> 10;

            if ( index != 1 )  {
                throw InstructionException( "JALI instruction has only one operand" );
            }

            break;

        case OP::ADD:
        case OP::SUB:
        case OP::MUL:
        case OP::DIV:
        case OP::AND:
        case OP::OR:
        case OP::XOR:
            operand1 = ( binary & 0x01C0 ) >> 6;
            operand2 = ( binary & 0x0038 ) >> 3;
            operand3 = ( binary & 0x0007 );
            break;

        case OP::NOT:
            operand1 = ( binary & 0x01C0 ) >> 6;
            operand2 = ( binary & 0x0038 ) >> 3;

            if ( index == 3 ) {
                throw InstructionException( "NOT instruction has no 3rd operand" );
            }

            break;

        case OP::HALT:
            throw InstructionException( "HALT instruction has no operands" );
            break;

        case OP::RET:
            operand1 = ( binary & 0x01C0 ) >> 6;

            if ( index != 1 ) {
                throw InstructionException( "RET instruction has only one operand" );
            }

            break;

        case OP::LI:
            operand1 = ( binary & 0x01C0 ) >> 6;

            if ( index != 1 ) {
                throw InstructionException( "LI instruction has only one operand" );
            }

            break;
    }

    switch ( index ) {
        case 1:
            return operand1;

        case 2:
            return operand2;

        case 3:
            return operand2;
    }
}

// std::uint8_t Instruction::get_operand(const uint8_t index) const{
//     if(index > 3 || index < 1){
//         throw InstructionException("Invalid operand index is requested");
//     }
//     data_t offset4 = (binary >> 9) & 15;
//     data_t offset3 = (binary >> 13) & 7;
//     uint8_t operand1 = (binary >> 10) & 7;
//     uint8_t operand2 = (binary >> 7) & 7;
//     uint8_t operand3;
//     if(offset3 != 7){
//         switch(offset3){
//             case 0:
//             case 1:
//             case 2:
//             case 5:
//                 if(index > 2){
//                     throw InstructionException("LW, SW and ANDI, JALR instructions have only two operands");
//                 }
//                 break;
//             case 3:
//             case 4:
//                 if(index > 2){
//                     throw InstructionException("BEQ and JALI instructions have only two operand");
//                 }
//                 break;
//             case 6:
//                 if(index > 1){
//                     throw InstructionException("JMP and JALI instructions have only one operand");
//                 }
//                 break;
//         }
//     }
//     else{
//         operand1 = (binary >> 6) & 7;
//         operand2 = (binary >> 3) & 7;
//         operand3 = binary & 3;
//         switch(offset4){
//             case 5:
//                 throw InstructionException("HALT instruction has no operands");
//                 break;
//             case 9:
//                 if(index > 1){
//                     throw InstructionException("RET instruction has only one operand");
//                 }
//         }
//     }
//     if((binary & 0xF800) == 0xF800){
//         if(index > 1 ){
//             throw InstructionException("LI instruction has only one operand");
//         }
//     }
//     if(index == 1){
//         return operand1;
//     }
//     else if(index == 2){
//         return operand2;
//     }
//     else{
//         return operand3;
//     }
//}

data_t Instruction::get_immediate() const {
    if ( type == OP::BEQ || type == OP::ADDI || type == OP::SW || type == OP::LW ) {
        // last 7 bits is immediate
        data_t val = binary & 0x7F;

        if ( val & 0x40 ) {
            // sign extend
            val |= 0xFF80; // MORE ONES
        }

        return val;
    } else if ( type == OP::JMP || type == OP::JALI ) {
        // last 10 bits is immediate
        data_t val = binary & 0x3FF;

        if ( val & 0x200 ) {
            // sign extend
            val |= 0xFC00; // LESS ONES !!!!!
        }

        return val;
    } else if ( type == OP::LI ) {
        data_t val = binary & 0x3F;

        if ( val & 0x20 ) {
            // sign extend
            val |= 0xFFC0;
        }

        return val;
    } else {
        throw InstructionException( "Instruction requested has no immediate value" );
    }


}


address_t Instruction::get_branch_addr() const {
    std::uint16_t imm = get_immediate();

    switch ( type ) {

        case OP::JALI:
            return  pc + imm + 2;

        case OP::BEQ:
            if ( ( imm & 0x8000 ) == 0x8000 ) {
                return  ( ( data_t ) pc ) + imm + 2; // if negative predict taken
            } else {
                return  pc + 2; // if positive predict not taken
            }

            break;

        default:
            throw InstructionException( "Can only compute for JALI and BEQ" );
    }
}
