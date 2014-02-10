#pragma once
#include "asm/internal/regex.hpp"
#include "asm/internal/internal.hpp"
#include "asm/internal/inst_helpers.hpp"
#include "logger.hpp"
#include <memory>
#include <string>

TEST_CASE( "Memory instruction parsing", "[regex]" ) {

    SECTION( "Load instructions" ) {

        SECTION( "spaces and tabs" ) {
            std::string noSpace( "LWR0,R7,22" );
            boost::smatch result;
            REQUIRE( boost::regex_match( noSpace, result, ass::regex::loadStore ) );
            REQUIRE( result[1] == "LW" );
            REQUIRE( result[2] == "R0" );
            REQUIRE( result[3] == "R7" );
            REQUIRE( result[4] == "22" );
            std::string spaces( "\t LW  R0\t,\t   R4\t,\t   \t42\t\t\t   " );
            REQUIRE( boost::regex_match( spaces, result, ass::regex::loadStore ) );
            REQUIRE( result[1] == "LW" );
            REQUIRE( result[2] == "R0" );
            REQUIRE( result[3] == "R4" );
            REQUIRE( result[4] == "42" );

            SECTION( "LW labels" ) {
                std::string label( "LW R0, R4, hello" );
                boost::smatch result;
                REQUIRE( boost::regex_match( label, result, ass::regex::loadStore ) );
                REQUIRE( result[1] == "LW" );
                REQUIRE( result[2] == "R0" );
                REQUIRE( result[3] == "R4" );
                REQUIRE( result[4] == "hello" );
            }


            SECTION( "SW labels" ) {
                std::string label( "SW R0, R4, hello" );
                boost::smatch result;
                REQUIRE( boost::regex_match( label, result, ass::regex::loadStore ) );
                REQUIRE( result[1] == "SW" );
                REQUIRE( result[2] == "R0" );
                REQUIRE( result[3] == "R4" );
                REQUIRE( result[4] == "hello" );
            }
        }

        SECTION( "Different Bases" ) {
            boost::smatch result;
            SECTION( "Binary literals" ) {
                std::string binary( "LW R0, R1, 0b01010110001" );
                REQUIRE( boost::regex_match( binary, result, ass::regex::loadStore ) );
                REQUIRE( result[1] == "LW" );
                REQUIRE( result[2] == "R0" );
                REQUIRE( result[3] == "R1" );
                REQUIRE( result[4] == "0b01010110001" );
            }
            SECTION( "HEX literals" ) {
                std::string hex( "LW R1, R2, 0xffaaCC0012456789BbdDeEFA" );
                REQUIRE( boost::regex_match( hex, result, ass::regex::loadStore ) );
                REQUIRE( result[1] == "LW" );
                REQUIRE( result[2] == "R1" );
                REQUIRE( result[3] == "R2" );
                REQUIRE( result[4] == "0xffaaCC0012456789BbdDeEFA" );
            }

            SECTION( "Base 10 literals" ) {
                std::string decimal( "LW R1, R2, 123456789" );
                REQUIRE( boost::regex_match( decimal, result, ass::regex::loadStore ) );
                REQUIRE( result[1] == "LW" );
                REQUIRE( result[2] == "R1" );
                REQUIRE( result[3] == "R2" );
                REQUIRE( result[4] == "123456789" );

            }

        }
    }


    SECTION( "Store instructions" ) {
        SECTION( "spaces and tabs" ) {
            std::string noSpace( "SWR0,R7,22" );
            boost::smatch result;
            REQUIRE( boost::regex_match( noSpace, result, ass::regex::loadStore ) );
            REQUIRE( result[1] == "SW" );
            REQUIRE( result[2] == "R0" );
            REQUIRE( result[3] == "R7" );
            REQUIRE( result[4] == "22" );
            std::string spaces( "\t SW  R0\t,\t   R4\t,\t   \t42\t\t\t   " );
            REQUIRE( boost::regex_match( spaces, result, ass::regex::loadStore ) );
            REQUIRE( result[1] == "SW" );
            REQUIRE( result[2] == "R0" );
            REQUIRE( result[3] == "R4" );
            REQUIRE( result[4] == "42" );
        }

        SECTION( "Different Bases" ) {
            boost::smatch result;
            SECTION( "Binary literals" ) {
                std::string binary( "SW R0, R1, 0b01010110001" );
                REQUIRE( boost::regex_match( binary, result, ass::regex::loadStore ) );
                REQUIRE( result[1] == "SW" );
                REQUIRE( result[2] == "R0" );
                REQUIRE( result[3] == "R1" );
                REQUIRE( result[4] == "0b01010110001" );
            }
            SECTION( "HEX literals" ) {
                std::string hex( "SW R1, R2, 0xffaaCC0012456789BbdDeEFA" );
                REQUIRE( boost::regex_match( hex, result, ass::regex::loadStore ) );
                REQUIRE( result[1] == "SW" );
                REQUIRE( result[2] == "R1" );
                REQUIRE( result[3] == "R2" );
                REQUIRE( result[4] == "0xffaaCC0012456789BbdDeEFA" );
            }

            SECTION( "Base 10 literals" ) {
                std::string decimal( "SW R1, R2, 123456789" );
                REQUIRE( boost::regex_match( decimal, result, ass::regex::loadStore ) );
                REQUIRE( result[1] == "SW" );
                REQUIRE( result[2] == "R1" );
                REQUIRE( result[3] == "R2" );
                REQUIRE( result[4] == "123456789" );

            }

        }
    }

}


TEST_CASE( "Directive instruction parsing", "[regex]" ) {
    SECTION( "ORIGIN" ) {
        std::string testdec( "#ORIGIN   \t 200" );
        boost::smatch result;
        REQUIRE( boost::regex_match( testdec, result, ass::regex::originDirect ) );
        REQUIRE( result[1] == "ORIGIN" );
        REQUIRE( result[2] == "200" );
        std::string testhex( "#ORIGIN 0xAF" );
        REQUIRE( boost::regex_match( testhex, result, ass::regex::originDirect ) );
        REQUIRE( result[1] == "ORIGIN" );
        REQUIRE( result[2] == "0xAF" );
        std::string testBinary( "#ORIGIN 0b1101" );
        REQUIRE( boost::regex_match( testBinary, result, ass::regex::originDirect ) );
        REQUIRE( result[1] == "ORIGIN" );
        REQUIRE( result[2] == "0b1101" );
    }

    SECTION( "EXPORT" ) {
        boost::smatch result;
        SECTION( "1 label" ) {
            std::string test( "#EXPORT hello" );
            REQUIRE( boost::regex_match( test, result, ass::regex::exportDirect ) );
            REQUIRE( result[1] == "EXPORT" );
            REQUIRE( result[2] == "hello" );
        }

        SECTION( "multi label" ) {
            std::string test( "#EXPORT hello,hell1, main, lol\t\t,\t label" );
            REQUIRE( boost::regex_match( test, result, ass::regex::exportDirect ) );
            REQUIRE( result[1] == "EXPORT" );
            REQUIRE( result[2] == "hello,hell1, main, lol\t\t,\t label" );
        }

        SECTION( "invalid multi label" ) {
            std::string test( "#EXPORT hello hell1, main, lol\t\t,\t label" );
            REQUIRE_FALSE( boost::regex_match( test, result, ass::regex::exportDirect ) );
        }
    }

    SECTION( "GLOBAL" ) {
        boost::smatch result;
        SECTION( "1 label" ) {
            std::string test( "#GLOBAL hello" );
            REQUIRE( boost::regex_match( test, result, ass::regex::globalDirect ) );
            REQUIRE( result[1] == "GLOBAL" );
            REQUIRE( result[2] == "hello" );
        }

        SECTION( "multi label" ) {
            std::string test( "#GLOBAL hello,hell1, main, lol\t\t,\t label" );
            REQUIRE( boost::regex_match( test, result, ass::regex::globalDirect ) );
            REQUIRE( result[1] == "GLOBAL" );
            REQUIRE( result[2] == "hello,hell1, main, lol\t\t,\t label" );
        }

        SECTION( "invalid multi label" ) {
            std::string test( "#GLOBAL hello hell1, main, lol\t\t,\t label" );
            REQUIRE_FALSE( boost::regex_match( test, result, ass::regex::globalDirect ) );
        }
    }

}

TEST_CASE( "Conditional branch instruction parsing", "[regex]" ) {

    boost::smatch result;
    SECTION( "BEQ" ) {
        SECTION( "NUMBER" ) {
            std::string test1( "BEQ R2, R1, 200" );
            REQUIRE( boost::regex_match( test1, result, ass::regex::branch ) );
            REQUIRE( result[1] == "BEQ" );
            REQUIRE( result[2] == "R2" );
            REQUIRE( result[3] == "R1" );
            REQUIRE( result[4] == "200" );
        }
        SECTION( "3 REGS" ) {
            std::string test2( "BEQ, R2, R1, R22" );
            REQUIRE_FALSE( boost::regex_match( test2, result, ass::regex::branch ) );
        }

        SECTION( "LABEL" ) {
            std::string test3( "BEQ R2, R1, hello" );
            REQUIRE( boost::regex_match( test3, result, ass::regex::branch ) );
            REQUIRE( result[1] == "BEQ" );
            REQUIRE( result[2] == "R2" );
            REQUIRE( result[3] == "R1" );
            REQUIRE( result[4] == "hello" );
        }
    }


}

TEST_CASE( "Logical instruction parsing", "[regex]" ) {
    boost::smatch result;
    SECTION( "NOT" ) {
        std::string test( "NOT R0, R1" );
        REQUIRE( boost::regex_match( test, result, ass::regex::notReg ) );
        REQUIRE( result[1] == "NOT" );
        REQUIRE( result[2] == "R0" );
        REQUIRE( result[3] == "R1" );
    }

    SECTION( "XOR" ) {
        std::string test( "XOR R1, R2, R3" );
        REQUIRE( boost::regex_match( test, result, ass::regex::arithmReg ) );
        REQUIRE( result[1] == "XOR" );
        REQUIRE( result[2] == "R1" );
        REQUIRE( result[3] == "R2" );
        REQUIRE( result[4] == "R3" );

    }

    SECTION( "AND" ) {
        std::string test( "AND R1, R2, R3" );
        REQUIRE( boost::regex_match( test, result, ass::regex::arithmReg ) );
        REQUIRE( result[1] == "AND" );
        REQUIRE( result[2] == "R1" );
        REQUIRE( result[3] == "R2" );
        REQUIRE( result[4] == "R3" );

    }

    SECTION( "OR" ) {
        std::string test( "OR R1, R2, R3" );
        REQUIRE( boost::regex_match( test, result, ass::regex::arithmReg ) );
        REQUIRE( result[1] == "OR" );
        REQUIRE( result[2] == "R1" );
        REQUIRE( result[3] == "R2" );
        REQUIRE( result[4] == "R3" );
    }



    SECTION( "XOR" ) {
        std::string test( "XOR R1, R2, R3" );
        REQUIRE( boost::regex_match( test, result, ass::regex::arithmReg ) );
        REQUIRE( result[1] == "XOR" );
        REQUIRE( result[2] == "R1" );
        REQUIRE( result[3] == "R2" );
        REQUIRE( result[4] == "R3" );

    }
}

TEST_CASE( "Special instructions", "[regex]" ) {
    boost::smatch result;
    SECTION( "HALT" ) {
        std::string test( "\tHALT    \t" );
        REQUIRE( boost::regex_match( test, result, ass::regex::halt ) );
        REQUIRE( result[1] == "HALT" );

    }
}
TEST_CASE( "Arithmetic instruction parsing", "[regex]" ) {
    boost::smatch result;
    SECTION( "ADD" ) {
        std::string test( "ADD R1, R2, R3" );
        REQUIRE( boost::regex_match( test, result, ass::regex::arithmReg ) );
        REQUIRE( result[1] == "ADD" );
        REQUIRE( result[2] == "R1" );
        REQUIRE( result[3] == "R2" );
        REQUIRE( result[4] == "R3" );

    }

    SECTION( "MUL" ) {
        std::string test( "MUL R1, R2, R3" );
        REQUIRE( boost::regex_match( test, result, ass::regex::arithmReg ) );
        REQUIRE( result[1] == "MUL" );
        REQUIRE( result[2] == "R1" );
        REQUIRE( result[3] == "R2" );
        REQUIRE( result[4] == "R3" );

    }

    SECTION( "DIV" ) {
        std::string test( "DIV R1, R2, R3" );
        REQUIRE( boost::regex_match( test, result, ass::regex::arithmReg ) );
        REQUIRE( result[1] == "DIV" );
        REQUIRE( result[2] == "R1" );
        REQUIRE( result[3] == "R2" );
        REQUIRE( result[4] == "R3" );

    }

    SECTION( "SUB" ) {
        std::string test( "SUB R1, R2, R3" );
        REQUIRE( boost::regex_match( test, result, ass::regex::arithmReg ) );
        REQUIRE( result[1] == "SUB" );
        REQUIRE( result[2] == "R1" );
        REQUIRE( result[3] == "R2" );
        REQUIRE( result[4] == "R3" );

    }

    SECTION( "ADDI" ) {
        SECTION( "NUMBER" ) {
            std::string test( "ADDI R1, R2, 0x04" );
            REQUIRE( boost::regex_match( test, result, ass::regex::arithmImm ) );
            REQUIRE( result[1] == "ADDI" );
            REQUIRE( result[2] == "R1" );
            REQUIRE( result[3] == "R2" );
            REQUIRE( result[4] == "0x04" );
        }
        SECTION( "LABEL" ) {
            std::string test( "ADDI R1, R2, hai" );
            REQUIRE( boost::regex_match( test, result, ass::regex::arithmImm ) );
            REQUIRE( result[1] == "ADDI" );
            REQUIRE( result[2] == "R1" );
            REQUIRE( result[3] == "R2" );
            REQUIRE( result[4] == "hai" );
        }
    }

    SECTION( "LI" ) {
        SECTION( "LABEL" ) {
            std::string test( "LI R1, data" );
            REQUIRE( boost::regex_match( test, result, ass::regex::loadImm ) );
            REQUIRE( result[1] == "LI" );
            REQUIRE( result[2] == "R1" );
            REQUIRE( result[3] == "data" );
        }
        SECTION( "NUMBER" ) {
            std::string test2( "LI R1, 22" );
            REQUIRE( boost::regex_match( test2, result, ass::regex::loadImm ) );
            REQUIRE( result[1] == "LI" );
            REQUIRE( result[2] == "R1" );
            REQUIRE( result[3] == "22" );
        }

    }
}

TEST_CASE( "Uncoditional branch instruction parsing", "[regex]" ) {
    boost::smatch result;
    SECTION( "RET" ) {
        std::string test( "RET R6" );
        REQUIRE( boost::regex_match( test, result, ass::regex::ret ) );
        REQUIRE( result[1] == "RET" );
        REQUIRE( result[2] == "R6" );
    }

    SECTION( "JMP" ) {

        SECTION( "NUMBER" ) {
            std::string test( "JMP R5, 22" );
            REQUIRE( boost::regex_match( test, result, ass::regex::jmp ) );
            REQUIRE( result[1] == "JMP" );
            REQUIRE( result[2] == "R5" );
            REQUIRE( result[3] == "22" );
        }
        SECTION( "LABLE" ) {
            std::string test( "JMP R5, hello" );
            REQUIRE( boost::regex_match( test, result, ass::regex::jmp ) );
            REQUIRE( result[1] == "JMP" );
            REQUIRE( result[2] == "R5" );
            REQUIRE( result[3] == "hello" );
        }
    }

    SECTION( "JAL" ) {
        SECTION( "JAL REG" ) {
            std::string test( "JAL R5, R2" );
            REQUIRE( boost::regex_match( test, result, ass::regex::jalReg ) );
            REQUIRE( result[1] == "JAL" );
            REQUIRE( result[2] == "R5" );
            REQUIRE( result[3] == "R2" );


        }

        SECTION( "JAL IMM" ) {

            SECTION( "NUMBER" ) {
                std::string test( "JAL R5, 22" );
                REQUIRE( boost::regex_match( test, result, ass::regex::jalImm ) );
                REQUIRE( result[1] == "JAL" );
                REQUIRE( result[2] == "R5" );
                REQUIRE( result[3] == "22" );

            }

            SECTION( "LABEL" ) {
                std::string test( "JAL R5, label" );
                REQUIRE( boost::regex_match( test, result, ass::regex::jalImm ) );
                REQUIRE( result[1] == "JAL" );
                REQUIRE( result[2] == "R5" );
                REQUIRE( result[3] == "label" );

            }
        }
    }
}


TEST_CASE( "Label Test", "[regex]" ) {
    boost::smatch result;
    std::string test( "hello:" );
    REQUIRE( boost::regex_match( test, result, ass::regex::labelReg ) );
    REQUIRE( result[1] == "hello" );

}


TEST_CASE( "BASIC CONVERSION TESTS", "[convertor]" ) {
    ass::internal::AssemblingStatus state( std::make_shared<BasicLogger>() );
    ass::internal::FileState &dummyState = state.getState( "I AM NOT A FILE" );
    std::size_t lineNum = 0;
    auto &insts = state.instList();
    const std::string testLine( "ADD R1, R2, R3" );
    REQUIRE( ass::internal::AddF()( testLine, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 1 );
    REQUIRE( insts.back().pc == 0 );
    REQUIRE( insts.back().type == ass::internal::InstType::ADD );
    REQUIRE( insts.back().data == 0xE053 );
    REQUIRE( lineNum == 1 );



    const std::string testADDI( "ADDI R0, R1, 20" );
    REQUIRE( ass::internal::AddiF()( testADDI, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 2 );
    REQUIRE( insts.back().pc == 2 );
    REQUIRE( insts.back().type == ass::internal::InstType::ADDI );
    REQUIRE( insts.back().data == 0x4094 );
    REQUIRE( lineNum == 2 );



    const std::string testAnd( "AND R1, R3, R4" );
    REQUIRE( ass::internal::AndF()( testAnd, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 3 );
    REQUIRE( insts.back().pc == 4 );
    REQUIRE( insts.back().type == ass::internal::InstType::AND );
    REQUIRE( insts.back().data == 0xEA5C );
    REQUIRE( lineNum == 3 );


    const std::string testBEQ( "BEQ R2, R0, 2" );
    REQUIRE( ass::internal::BEQF()( testBEQ, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 4 );
    REQUIRE( insts.back().pc == 6 );
    REQUIRE( insts.back().type == ass::internal::InstType::BEQ );
    // REQUIRE(insts.back().data == 0x800F);
    REQUIRE( lineNum == 4 );

    const std::string testDiv( "DIV R1, R1, R1" );
    REQUIRE( ass::internal::DivF()( testDiv, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 5 );
    REQUIRE( insts.back().pc == 8 );
    REQUIRE( insts.back().type == ass::internal::InstType::DIV );
    REQUIRE( insts.back().data == 0xE649 );
    REQUIRE( lineNum == 5 );



    const std::string testHalt( "HALT" );
    REQUIRE( ass::internal::HaltF()( testHalt, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 6 );
    REQUIRE( insts.back().pc == 10 );
    REQUIRE( insts.back().type == ass::internal::InstType::HALT );
    REQUIRE( insts.back().data == 0xE800 );
    REQUIRE( lineNum == 6 );



    const std::string testJALI( "JAL R7, 0b1111111111" );
    REQUIRE( ass::internal::JALIF()( testJALI, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 7 );
    REQUIRE( insts.back().pc == 12 );
    REQUIRE( insts.back().type == ass::internal::InstType::JALI );
    REQUIRE( insts.back().data == ( 0xDFFF - insts.back().pc ) );
    REQUIRE( lineNum == 7 );

    const std::string testJALR( "JAL R2, R3" );
    REQUIRE( ass::internal::JALRF()( testJALR, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 8 );
    REQUIRE( insts.back().pc == 14 );
    REQUIRE( insts.back().type == ass::internal::InstType::JALR );
    REQUIRE( insts.back().data == 0xA980 );
    REQUIRE( lineNum == 8 );

    const std::string testJMP( "JMP R0, 42" );
    REQUIRE( ass::internal::JMPF()( testJMP, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 9 );
    REQUIRE( insts.back().pc == 16 );
    REQUIRE( insts.back().type == ass::internal::InstType::JMP );
    REQUIRE( insts.back().data == 0x602A - insts.back().pc );
    REQUIRE( lineNum == 9 );

    const std::string liTest( "LI R5, 42" );
    REQUIRE( ass::internal::LIF()( liTest, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 10 );
    REQUIRE( insts.back().pc == 18 );
    REQUIRE( insts.back().type == ass::internal::InstType::LI );
    REQUIRE( insts.back().data == 0xF56A );
    REQUIRE( lineNum == 10 );

    const std::string LWTEST( "LW R1, R2, 17" );
    REQUIRE( ass::internal::LSWF()( LWTEST, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 11 );
    REQUIRE( insts.back().pc == 20 );
    REQUIRE( insts.back().type == ass::internal::InstType::LW );
    REQUIRE( insts.back().data == 0x0511 );
    REQUIRE( lineNum == 11 );

    const std::string SWTEST( "SW R1, R2, 17" );
    REQUIRE( ass::internal::LSWF()( SWTEST, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 12 );
    REQUIRE( insts.back().pc == 22 );
    REQUIRE( insts.back().type == ass::internal::InstType::SW );
    REQUIRE( insts.back().data == 0x2511 );
    REQUIRE( lineNum == 12 );

    const std::string mulTest( "MUL R1, R1, R1" );
    REQUIRE( ass::internal::MulF()( mulTest, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 13 );
    REQUIRE( insts.back().pc == 24 );
    REQUIRE( insts.back().type == ass::internal::InstType::MUL );
    REQUIRE( insts.back().data == 0xE449 );
    REQUIRE( lineNum == 13 );

    const std::string notTest( "NOT R1, R0" );
    REQUIRE( ass::internal::NOTF()( notTest, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 14 );
    REQUIRE( insts.back().pc == 26 );
    REQUIRE( insts.back().type == ass::internal::InstType::NOT );
    REQUIRE( insts.back().data == 0xF040 );
    REQUIRE( lineNum == 14 );

    const std::string orTest( "OR R2, R2, R2" );
    REQUIRE( ass::internal::ORF()( orTest, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 15 );
    REQUIRE( insts.back().pc == 28 );
    REQUIRE( insts.back().type == ass::internal::InstType::OR );
    REQUIRE( insts.back().data == 0xEC92 );
    REQUIRE( lineNum == 15 );

    const std::string retTest( "RET R7" );
    REQUIRE( ass::internal::RETF()( retTest, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 16 );
    REQUIRE( insts.back().pc == 30 );
    REQUIRE( insts.back().type == ass::internal::InstType::RET );
    REQUIRE( insts.back().data == 0xF3C0 );
    REQUIRE( lineNum == 16 );

    const std::string subTest( "SUB R0, R0, R0" );
    REQUIRE( ass::internal::SubF()( subTest, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 17 );
    REQUIRE( insts.back().pc == 32 );
    REQUIRE( insts.back().type == ass::internal::InstType::SUB );
    REQUIRE( insts.back().data == 0b1110001000000000 );
    REQUIRE( lineNum == 17 );

    const std::string xorTest( "XOR R1, R2, R3" );
    REQUIRE( ass::internal::XORF()( xorTest, state, lineNum, dummyState ) );
    REQUIRE( insts.size() == 18 );
    REQUIRE( insts.back().pc == 34 );
    REQUIRE( insts.back().type == ass::internal::InstType::XOR );
    REQUIRE( insts.back().data == 0b1110111001010011 );
    REQUIRE( lineNum == 18 );

}
