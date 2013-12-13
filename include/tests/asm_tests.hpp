#pragma once
#include "asm/internal/regex.hpp"
#include <string>

TEST_CASE("Memory instruction parsing", "[regex]") {

    SECTION("Load instructions") {
        
        SECTION("spaces and tabs") {
            std::string noSpace("LWR0,R7,22");
            boost::smatch result;
            REQUIRE(boost::regex_match(noSpace, result, ass::regex::loadStore));
            REQUIRE(result[1] == "LW");
            REQUIRE(result[2] == "R0");
            REQUIRE(result[3] == "R7");
            REQUIRE(result[4] == "22");
            std::string spaces("\t LW  R0\t,\t   R4\t,\t   \t42\t\t\t   ");
            REQUIRE(boost::regex_match(spaces, result, ass::regex::loadStore));
            REQUIRE(result[1] == "LW");
            REQUIRE(result[2] == "R0");
            REQUIRE(result[3] == "R4");
            REQUIRE(result[4] == "42");
        }

        SECTION("Different Bases") {
            boost::smatch result;
            SECTION("Binary literals") {
                std::string binary("LW R0, R1, 0b01010110001");
                REQUIRE(boost::regex_match(binary, result, ass::regex::loadStore));
                REQUIRE(result[1] == "LW");
                REQUIRE(result[2] == "R0");
                REQUIRE(result[3] == "R1");
                REQUIRE(result[4] == "0b01010110001");
            }
            SECTION("HEX literals") {
                std::string hex("LW R1, R2, 0xffaaCC0012456789BbdDeEFA");
                REQUIRE(boost::regex_match(hex, result, ass::regex::loadStore));
                REQUIRE(result[1] == "LW");
                REQUIRE(result[2] == "R1");
                REQUIRE(result[3] == "R2");
                REQUIRE(result[4] == "0xffaaCC0012456789BbdDeEFA");
            }
            
            SECTION("Base 10 literals") {
                std::string decimal("LW R1, R2, 123456789");
                REQUIRE(boost::regex_match(decimal, result, ass::regex::loadStore));
                REQUIRE(result[1] == "LW");
                REQUIRE(result[2] == "R1");
                REQUIRE(result[3] == "R2");
                REQUIRE(result[4] == "123456789");

            }

        }
    }


    SECTION("Store instructions") {
        SECTION("spaces and tabs") {
            std::string noSpace("SWR0,R7,22");
            boost::smatch result;
            REQUIRE(boost::regex_match(noSpace, result, ass::regex::loadStore));
            REQUIRE(result[1] == "SW");
            REQUIRE(result[2] == "R0");
            REQUIRE(result[3] == "R7");
            REQUIRE(result[4] == "22");
            std::string spaces("\t SW  R0\t,\t   R4\t,\t   \t42\t\t\t   ");
            REQUIRE(boost::regex_match(spaces, result, ass::regex::loadStore));
            REQUIRE(result[1] == "SW");
            REQUIRE(result[2] == "R0");
            REQUIRE(result[3] == "R4");
            REQUIRE(result[4] == "42");
        }

        SECTION("Different Bases") {
            boost::smatch result;
            SECTION("Binary literals") {
                std::string binary("SW R0, R1, 0b01010110001");
                REQUIRE(boost::regex_match(binary, result, ass::regex::loadStore));
                REQUIRE(result[1] == "SW");
                REQUIRE(result[2] == "R0");
                REQUIRE(result[3] == "R1");
                REQUIRE(result[4] == "0b01010110001");
            }
            SECTION("HEX literals") {
                std::string hex("SW R1, R2, 0xffaaCC0012456789BbdDeEFA");
                REQUIRE(boost::regex_match(hex, result, ass::regex::loadStore));
                REQUIRE(result[1] == "SW");
                REQUIRE(result[2] == "R1");
                REQUIRE(result[3] == "R2");
                REQUIRE(result[4] == "0xffaaCC0012456789BbdDeEFA");
            }
            
            SECTION("Base 10 literals") {
                std::string decimal("SW R1, R2, 123456789");
                REQUIRE(boost::regex_match(decimal, result, ass::regex::loadStore));
                REQUIRE(result[1] == "SW");
                REQUIRE(result[2] == "R1");
                REQUIRE(result[3] == "R2");
                REQUIRE(result[4] == "123456789");

            }

        }
    }
    
}


TEST_CASE("Directive instruction parsing", "[regex]") {
    SECTION("ORIGIN") {
        std::string testdec("#ORIGIN   \t 200");
        boost::smatch result;
        REQUIRE(boost::regex_match(testdec, result, ass::regex::originDirect));
        REQUIRE(result[1] == "ORIGIN");
        REQUIRE(result[2] == "200");
        std::string testhex("#ORIGIN 0xAF");
        REQUIRE(boost::regex_match(testhex, result, ass::regex::originDirect));
        REQUIRE(result[1] == "ORIGIN");
        REQUIRE(result[2] == "0xAF");
        std::string testBinary("#ORIGIN 0b1101");
        REQUIRE(boost::regex_match(testBinary, result, ass::regex::originDirect));
        REQUIRE(result[1] == "ORIGIN");
        REQUIRE(result[2] == "0b1101");        
    }

    SECTION("EXPORT") {
        boost::smatch result;
        SECTION("1 label") {
            std::string test("#EXPORT hello");
            REQUIRE(boost::regex_match(test, result, ass::regex::exportDirect));
            REQUIRE(result[1] == "EXPORT");
            REQUIRE(result[2] == "hello");
        }

        SECTION("multi label") {
            std::string test("#EXPORT hello,hell1, main, lol\t\t,\t label");
            REQUIRE(boost::regex_match(test, result, ass::regex::exportDirect));
            REQUIRE(result[1] == "EXPORT");
            REQUIRE(result[2] == "hello,hell1, main, lol\t\t,\t label");            
        }

        SECTION("invalid multi label") {
            std::string test("#EXPORT hello hell1, main, lol\t\t,\t label");
            REQUIRE_FALSE(boost::regex_match(test, result, ass::regex::exportDirect));
        }
    }

    SECTION("GLOBAL") {
        boost::smatch result;
        SECTION("1 label") {
            std::string test("#GLOBAL hello");
            REQUIRE(boost::regex_match(test, result, ass::regex::globalDirect));
            REQUIRE(result[1] == "GLOBAL");
            REQUIRE(result[2] == "hello");
        }

        SECTION("multi label") {
            std::string test("#GLOBAL hello,hell1, main, lol\t\t,\t label");
            REQUIRE(boost::regex_match(test, result, ass::regex::globalDirect));
            REQUIRE(result[1] == "GLOBAL");
            REQUIRE(result[2] == "hello,hell1, main, lol\t\t,\t label");            
        }

        SECTION("invalid multi label") {
            std::string test("#GLOBAL hello hell1, main, lol\t\t,\t label");
            REQUIRE_FALSE(boost::regex_match(test, result, ass::regex::globalDirect));
        }
    }

}

TEST_CASE("Conditional branch instruction parsing", "[regex]") {
    
    boost::smatch result;
    SECTION("BEQ") {
        std::string test1("BEQ R2, R1, R3");
        REQUIRE(boost::regex_match(test1, result, ass::regex::branch));
        REQUIRE(result[1] == "BEQ");
        REQUIRE(result[2] == "R2");
        REQUIRE(result[3] == "R1");
        REQUIRE(result[4] == "R3");

        std::string test2("BEQ, R2, R1, R22");
        REQUIRE_FALSE(boost::regex_match(test2, result, ass::regex::branch));        

    }

    SECTION("BNE") {
        std::string test1("BNE R2, R1, R3");
        REQUIRE(boost::regex_match(test1, result, ass::regex::branch));
        REQUIRE(result[1] == "BNE");
        REQUIRE(result[2] == "R2");
        REQUIRE(result[3] == "R1");
        REQUIRE(result[4] == "R3");

        std::string test2("BNE R2, R1");
        REQUIRE_FALSE(boost::regex_match(test2, result, ass::regex::branch));  

    }

    SECTION("BGT") {
        std::string test1("BGT R2, R1, R3");
        REQUIRE(boost::regex_match(test1, result, ass::regex::branch));
        REQUIRE(result[1] == "BGT");
        REQUIRE(result[2] == "R2");
        REQUIRE(result[3] == "R1");
        REQUIRE(result[4] == "R3");

        std::string test2("BGT");
        REQUIRE_FALSE(boost::regex_match(test2, result, ass::regex::branch));  


    }

    SECTION("BLT") {
        std::string test1("BLT R2, R1, R3");
        REQUIRE(boost::regex_match(test1, result, ass::regex::branch));
        REQUIRE(result[1] == "BLT");
        REQUIRE(result[2] == "R2");
        REQUIRE(result[3] == "R1");
        REQUIRE(result[4] == "R3");

        std::string test2("BLT, R2, R1, R0");
        REQUIRE_FALSE(boost::regex_match(test2, result, ass::regex::branch));  

    }

    SECTION("BGE") {
        std::string test1("BGE R2, R1, R3");
        REQUIRE(boost::regex_match(test1, result, ass::regex::branch));
        REQUIRE(result[1] == "BGE");
        REQUIRE(result[2] == "R2");
        REQUIRE(result[3] == "R1");
        REQUIRE(result[4] == "R3");

        std::string test2("BGE");
        REQUIRE_FALSE(boost::regex_match(test2, result, ass::regex::branch));  

    }

    SECTION("BLE") {
        std::string test1("BLE R2, R1, R3");
        REQUIRE(boost::regex_match(test1, result, ass::regex::branch));
        REQUIRE(result[1] == "BLE");
        REQUIRE(result[2] == "R2");
        REQUIRE(result[3] == "R1");
        REQUIRE(result[4] == "R3");

        std::string test2("BLE");
        REQUIRE_FALSE(boost::regex_match(test2, result, ass::regex::branch));  

    }
}

TEST_CASE("Logical instruction parsing", "[regex]") {
    boost::smatch result;
    SECTION("NOT") {
        std::string test("NOT R0, R1");
        REQUIRE(boost::regex_match(test, result, ass::regex::notReg));
        REQUIRE(result[1] == "NOT");
        REQUIRE(result[2] == "R0");
        REQUIRE(result[3] == "R1");        
    }

    SECTION("XOR") {
        std::string test("XOR R1, R2, R3");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmReg));
        REQUIRE(result[1] == "XOR");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "R2");
        REQUIRE(result[4] == "R3");

    }

    SECTION("AND") {
        std::string test("AND R1, R2, R3");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmReg));
        REQUIRE(result[1] == "AND");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "R2");
        REQUIRE(result[4] == "R3");

    }

    SECTION("OR") {
        std::string test("OR R1, R2, R3");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmReg));
        REQUIRE(result[1] == "OR");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "R2");
        REQUIRE(result[4] == "R3");
    }

      SECTION("NAND") {
        std::string test("NAND R1, R2, R3");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmReg));
        REQUIRE(result[1] == "NAND");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "R2");
        REQUIRE(result[4] == "R3");

    }

       SECTION("XOR") {
        std::string test("XOR R1, R2, R3");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmReg));
        REQUIRE(result[1] == "XOR");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "R2");
        REQUIRE(result[4] == "R3");

    }
}

TEST_CASE("Arithmetic instruction parsing", "[regex]") {
    boost::smatch result;
    SECTION("ADD") {
        std::string test("ADD R1, R2, R3");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmReg));
        REQUIRE(result[1] == "ADD");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "R2");
        REQUIRE(result[4] == "R3");

    }

    SECTION("MUL") {
        std::string test("MUL R1, R2, R3");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmReg));
        REQUIRE(result[1] == "MUL");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "R2");
        REQUIRE(result[4] == "R3");

    }

    SECTION("DIV") {
        std::string test("DIV R1, R2, R3");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmReg));
        REQUIRE(result[1] == "DIV");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "R2");
        REQUIRE(result[4] == "R3");

    }

    SECTION("SUB") {
        std::string test("SUB R1, R2, R3");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmReg));
        REQUIRE(result[1] == "SUB");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "R2");
        REQUIRE(result[4] == "R3");

    }

    SECTION("ADDI") {
        std::string test("ADDI R1, 0x04");
        REQUIRE(boost::regex_match(test, result, ass::regex::arithmImm));
        REQUIRE(result[1] == "ADDI");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "0x04");

    }

      SECTION("LI") {
        std::string test("LI R1, data");
        REQUIRE(boost::regex_match(test, result, ass::regex::loadImm));
        REQUIRE(result[1] == "LI");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "data");
        std::string test2("LI R1, 22");
        REQUIRE(boost::regex_match(test2, result, ass::regex::loadImm));
        REQUIRE(result[1] == "LI");
        REQUIRE(result[2] == "R1");
        REQUIRE(result[3] == "22");

    }
}

TEST_CASE("Uncoditional branch instruction parsing", "[regex]") {
    boost::smatch result;
    SECTION("RET") {
        std::string test("RET R6");
        REQUIRE(boost::regex_match(test, result, ass::regex::ret));
        REQUIRE(result[1] == "RET");
        REQUIRE(result[2] == "R6");
    }

    SECTION("JMP") {

        SECTION("NUMBER") {
            std::string test("JMP R5, 22");
            REQUIRE(boost::regex_match(test, result, ass::regex::jmp));
            REQUIRE(result[1] == "JMP");
            REQUIRE(result[2] == "R5");
            REQUIRE(result[3] == "22");
        }

    }

    SECTION("JAL") {
        SECTION("JAL REG") {
            std::string test("JAL R5, R2");
            REQUIRE(boost::regex_match(test, result, ass::regex::jalReg));
            REQUIRE(result[1] == "JAL");
            REQUIRE(result[2] == "R5");
            REQUIRE(result[3] == "R2");


        }

        SECTION("JAL IMM") {

            SECTION("NUMBER") {
                std::string test("JAL R5, 22");
                REQUIRE(boost::regex_match(test, result, ass::regex::jalImm));
                REQUIRE(result[1] == "JAL");
                REQUIRE(result[2] == "R5");
                REQUIRE(result[3] == "22");

            }

            SECTION("LABEL") {
                std::string test("JAL R5, label");
                REQUIRE(boost::regex_match(test, result, ass::regex::jalImm));
                REQUIRE(result[1] == "JAL");
                REQUIRE(result[2] == "R5");
                REQUIRE(result[3] == "label");

            }
        }
    }
}


TEST_CASE("Label Test", "[regex]") {
    boost::smatch result;
    std::string test("hello:");
    REQUIRE(boost::regex_match(test, result, ass::regex::labelReg));
    REQUIRE(result[1] == "hello");  

}

