#pragma once
#include "core/instruction.hpp"

TEST_CASE( "Decode" ) {
    SECTION( "ADD instructions" ) {
        Instruction i1( 0xE053, 0x0000 ); //ADD R1,R2,R3
        Instruction i2( 0xE253, 0x0002 ); //SUB R1,R2,R3
        Instruction i3( 0x4985, 0x0004 ); //ADDI R2,R3,#5
        Instruction i4( 0xF445, 0x0006 ); //LI R1,#5
        SECTION( "OPCODE DECODE" ) {
            REQUIRE( i1.type == OP::ADD );
            REQUIRE( i2.type == OP::SUB );
            REQUIRE( i3.type == OP::ADDI );
            REQUIRE( i4.type == OP::LI );
        }
        SECTION( "OPERAND DECODE" ) {
            REQUIRE( i1.get_operand( 1 ) == 1 );
            REQUIRE( i1.get_operand( 2 ) == 2 );
            REQUIRE( i1.get_operand( 3 ) == 3 );

            REQUIRE( i2.get_operand( 1 ) == 1 );
            REQUIRE( i2.get_operand( 2 ) == 2 );
            REQUIRE( i2.get_operand( 3 ) == 3 );

            REQUIRE( i3.get_operand( 1 ) == 2 );
            REQUIRE( i3.get_operand( 2 ) == 3 );
            REQUIRE( i3.get_immediate() == 5 );

            REQUIRE( i4.get_operand( 1 ) == 1 );
            REQUIRE( i4.get_immediate() == 5 );
        }
    }

    SECTION( "MULL instructions" ) {
        Instruction i5( 0xE453, 0x0008 ); //MULL R1,R2,R3
        Instruction i6( 0xE653, 0x000A ); //DIV R1,R2,R3
        SECTION( "OPCODE DECODE" ) {
            REQUIRE( i5.type == OP::MUL );
            REQUIRE( i6.type == OP::DIV );
        }
        SECTION( "OPERAND DECODE" ) {
            REQUIRE( i5.get_operand( 1 ) == 1 );
            REQUIRE( i5.get_operand( 2 ) == 2 );
            REQUIRE( i5.get_operand( 3 ) == 3 );

            REQUIRE( i6.get_operand( 2 ) == 2 );
            REQUIRE( i6.get_operand( 2 ) == 2 );
            REQUIRE( i6.get_operand( 3 ) == 3 );
        }
    }

    SECTION( "LOAD instructions" ) {
        Instruction i7( 0x0508, 0x000C ); //LW R1,M[8]
        Instruction i8( 0x2508, 0x000E ); //SW R1,M[8]
        SECTION( "OPCODE DECODE" ) {
            REQUIRE( i7.type == OP::LW );
            REQUIRE( i8.type == OP::SW );
        }
        SECTION( "OPERAND DECODE" ) {
            REQUIRE( i7.get_operand( 1 ) == 1 );
            REQUIRE( i7.get_immediate() == 8 );

            REQUIRE( i8.get_operand( 1 ) == 1 );
            REQUIRE( i8.get_immediate() == 8 );
        }
    }

    SECTION( "LOGICAL instructions" ) {
        Instruction i9( 0xEA53, 0x0010 ); //AND R1,R2,R3
        Instruction i10( 0xEC53, 0x0012 ); //OR R1,R2,R3
        Instruction i11( 0xEE53, 0x0014 ); //XOR R1,R2,R3
        Instruction i12( 0xF053, 0x0016 ); //NOT R1,R2,R3
        SECTION( "OPCODE DECODE" ) {
            REQUIRE( i9.type == OP::AND );
            REQUIRE( i10.type == OP::OR );
            REQUIRE( i11.type == OP::XOR );
            REQUIRE( i12.type == OP::NOT );
        }
        SECTION( "OPERAND DECODE" ) {
            REQUIRE( i9.get_operand( 1 ) == 1 );
            REQUIRE( i9.get_operand( 2 ) == 2 );
            REQUIRE( i9.get_operand( 3 ) == 3 );

            REQUIRE( i10.get_operand( 1 ) == 1 );
            REQUIRE( i10.get_operand( 2 ) == 2 );
            REQUIRE( i10.get_operand( 3 ) == 3 );

            REQUIRE( i11.get_operand( 1 ) == 1 );
            REQUIRE( i11.get_operand( 2 ) == 2 );
            REQUIRE( i11.get_operand( 3 ) == 3 );

            REQUIRE( i12.get_operand( 1 ) == 1 );
            REQUIRE( i12.get_operand( 2 ) == 2 );
            REQUIRE( i12.get_operand( 3 ) == 3 );
        }
    }

    SECTION( "BRANCH instructions" ) {
        Instruction i13( 0x64FF, 0x0018 ); //JMP R1,#255
        Instruction i14( 0xA50F, 0x001A ); //JALR R1,R2
        Instruction i15( 0xC4FF, 0x001C ); //JALI R1,#255
        Instruction i16( 0xF240, 0x001E ); //RET R1
        Instruction i17( 0x850F, 0x0020 ); //BEQ R1,R2,#15
        SECTION( "OPCODE DECODE" ) {
            REQUIRE( i13.type == OP::JMP );
            REQUIRE( i14.type == OP::JALR );
            REQUIRE( i15.type == OP::JALI );
            REQUIRE( i16.type == OP::RET );
            REQUIRE( i17.type == OP::BEQ );
        }
        SECTION( "OPERAND DECODE" ) {
            REQUIRE( i13.get_operand( 1 ) == 1 );
            REQUIRE( i13.get_immediate() == 255 );

            REQUIRE( i14.get_operand( 1 ) == 1 );
            REQUIRE( i14.get_operand( 2 ) == 2 );

            REQUIRE( i15.get_operand( 1 ) == 1 );
            REQUIRE( i15.get_immediate() == 255 );

            REQUIRE( i16.get_operand( 1 ) == 1 );

            REQUIRE( i17.get_operand( 1 ) == 1 );
            REQUIRE( i17.get_operand( 2 ) == 2 );
            REQUIRE( i17.get_immediate() == 15 );
        }

        Instruction i18( 0x857F, 0x0021 ); //BEQ R1,R2,-1
        SECTION( "PREDICTED BRANCH ADDRESS" ) {
            REQUIRE( i15.get_branch_addr() == 285 );
            REQUIRE( i17.get_branch_addr() == 0x0020 + 0x0002 );
            REQUIRE( i18.get_branch_addr() == -1 + 0x0021 + 0x0002 );
        }
    }

    SECTION( "HALT instruction" ) {
        Instruction i18( 0xE800, 0x0021 ); //HALT
        REQUIRE( i18.type == OP::HALT );
    }
}