#include "asm/regex.hpp"

using namespace ass;
/**
 * @brief matches base 10, 16 and 2 numbers.
 */
static std::string number(R"((0x([0-9]|[a-f]|[A-F])+|0b[0-1]+|[0-9]+))");

 const boost::regex regex::loadStore
                  (R"(\s*(LW|SW)\s*(R[0-7])\s*,\s*(R[0-7])\s*,\s*()" + number + R"()\s*)"); 