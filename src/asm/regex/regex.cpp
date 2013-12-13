#include "asm/regex.hpp"
#include <string>
using namespace ass;
/**
 * @brief matches base 10, 16 and 2 numbers.
 */
static const std::string number(R"((0x([0-9]|[a-f]|[A-F])+|0b[0-1]+|[0-9]+))");
/**
 * @brief matches labels
 */
static const std::string label(R"([a-z][a-z0-9]*)");
/**
 * @brief matches one or more label.
 */
static const std::string labelList(R"((\s*)" + label + R"(\s*,)*\s*)" + label);



const boost::regex regex::originDirect(std::string(R"(\s*#(ORIGIN)\s*()") + number + R"()\s*)" );
const boost::regex regex::exportDirect(R"(\s*#(EXPORT)\s*()" + labelList + R"()\s*)");
const boost::regex regex::globalDirect(R"(\s*#(GLOBAL)\s*()" + labelList + R"()\s*)");




const boost::regex regex::jmp(R"(\s*(JMP)\s*(R[0-7])\s*,\s*()" + number + R"())");
const boost::regex regex::jalImm(R"(\s*(JAL)\s*(R[0-7])\s*,\s*(()" + label + ")|" + number + R"()\s*)" );
const boost::regex regex::jalReg(R"(\s*(JAL)\s*(R[0-7])\s*,\s*(R[0-7])\s*)"); 


const boost::regex regex::arithmImm(R"(\s*(ADDI)\s*(R[0-7])\s*,\s*(()" + label + ")|" + number + R"()\s*)" );
const boost::regex regex::loadImm(R"(\s*(LI)\s*(R[0-7])\s*,\s*(()" + label + ")|" + number + R"()\s*)" );


const boost::regex regex::ret(R"(\s*(RET)\s*(R[0-7])\s*)");
const boost::regex regex::notReg(R"(\s*(NOT)\s*(R[0-7])\s*,\s*(R[0-7])\s*)");

const boost::regex regex::loadStore(R"(\s*(LW|SW)\s*(R[0-7])\s*,\s*(R[0-7])\s*,\s*()" + number + R"()\s*)");               
const boost::regex regex::branch(R"(\s*(BEQ|BNE|BGT|BLT|BGE|BLE)\s*(R[0-7])\s*,\s*(R[0-7])\s*,\s*(R[0-7])\s*)");

const boost::regex regex::arithmReg(R"(\s*(ADD|SUB|MUL|DIV|AND|OR|NAND|XOR)\s*)" +
                std::string(R"((R[0-7])\s*,\s*(R[0-7])\s*,\s*(R[0-7])\s*)" ));



                  
const boost::regex regex::labelReg(R"(\s*()" + label + R"()\s*:\s*)");


