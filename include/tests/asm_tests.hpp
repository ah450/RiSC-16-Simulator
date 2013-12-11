#pragma once
#include "asm/regex.hpp"
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

