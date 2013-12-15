#pragma once
#include <boost/regex.hpp>
/**
 * Regular expressions used by assembler
 */

namespace ass { namespace regex {


/**
 * @brief LW/SW Rd, Rs, number.
 */
extern const boost::regex loadStore;

/**
 * @brief JMP rd, number
 */
extern const boost::regex jmp;

/**
 * @brief JAL rega, regb
 */
extern const boost::regex jalReg;
/**
 * @brief JAL rega, label|number
 */
extern const boost::regex jalImm;

/**
 * @brief conditional branching form BEQ RA, RB, RT : branches to RT.
 * @details BEQ, BGT, BLT, BGE, BLE, BNE.
 */
extern const boost::regex branch;
/**
 * @brief return instruction "RET RX".
 */
extern const boost::regex ret;
/**
 * @brief arithmetic and logical operations that deal with 3 registers e.g ADD RD, RS, RT
 */
extern const boost::regex arithmReg;
/**
 * @brief register instructions that deal with registers and immediete.
 * @details  ADDI
 */
extern const boost::regex arithmImm;
/**
 * @brief LI RD, label|number
 */
extern const boost::regex loadImm;


extern const boost::regex originDirect;
extern const boost::regex exportDirect;
extern const boost::regex globalDirect;

/**
 * @brief label declarations
 */
extern const boost::regex labelReg;
/**
 * @brief NOT RD, RS
 */
extern const boost::regex notReg;

extern const boost::regex halt;

}
}