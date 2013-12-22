#ifndef REG_FILE_HPP
#define REG_FILE_HPP
#include <array>

struct reg_file{
    std::array<std::uint16_t,8> regs;
    std::uint16_t pc; //program counter register
};

#endif // REG_FILE_HPP
