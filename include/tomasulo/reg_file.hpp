#ifndef REG_FILE_HPP
#define REG_FILE_HPP
#include <array>

struct reg_file{
    std::array<std::uint16_t,8> regs;
};

#endif // REG_FILE_HPP
