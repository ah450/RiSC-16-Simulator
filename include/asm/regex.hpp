#pragma once
#include <boost/regex.hpp>
/**
 * Regular expressions used by assembler
 */

namespace ass { namespace regex {



extern const boost::regex loadStore;
extern const boost::regex jmpImm;
extern const boost::regex jmpLabel;
extern const boost::regex branchImm;
extern const boost::regex branchLabel;
extern const boost::regex jalReg;
extern const boost::regex jalLabel;
extern const boost::regex ret;
extern const boost::regex arithmTernary;
extern const boost::regex arithmBinary;

}
}