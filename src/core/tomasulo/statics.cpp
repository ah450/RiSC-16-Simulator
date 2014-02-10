#include "core/typedefs.hpp"
#include "core/utility.hpp"
#include <unordered_map>
#include <utility>




const std::unordered_map<OP, FUNCTION_UNIT> OP_MAPING =  {
    {
        {std::make_pair( OP::LW, FUNCTION_UNIT::LOAD )}, {std::make_pair( OP::SW, FUNCTION_UNIT::LOAD )},
        {std::make_pair( OP::ADDI, FUNCTION_UNIT::ADD )}, {std::make_pair( OP::JMP, FUNCTION_UNIT::BRANCH )},
        {std::make_pair( OP::LI, FUNCTION_UNIT::ADD )}, {std::make_pair( OP::JALR, FUNCTION_UNIT::BRANCH )},
        {std::make_pair( OP::JALI, FUNCTION_UNIT::BRANCH )}, {std::make_pair( OP::ADD, FUNCTION_UNIT::ADD )},
        {std::make_pair( OP::SUB, FUNCTION_UNIT::ADD )}, {std::make_pair( OP::MUL, FUNCTION_UNIT::MULL )},
        {std::make_pair( OP::DIV, FUNCTION_UNIT::MULL )}, {std::make_pair( OP::HALT, FUNCTION_UNIT::BRANCH )},
        {std::make_pair( OP::AND, FUNCTION_UNIT::LOGIC )}, {std::make_pair( OP::OR, FUNCTION_UNIT::LOGIC )},
        {std::make_pair( OP::XOR, FUNCTION_UNIT::LOGIC )}, {std::make_pair( OP::NOT, FUNCTION_UNIT::LOGIC )},
        {std::make_pair( OP::RET, FUNCTION_UNIT::BRANCH )}, {std::make_pair( OP::BEQ, FUNCTION_UNIT::BRANCH )}
    }
};