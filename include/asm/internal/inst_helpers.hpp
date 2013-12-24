#pragma once
#include "asm/internal/utility.hpp"
#include "asm/internal/internal.hpp"
#include <functional>
#include <memory>
#include <array>

//TODO:GENERATE OPCODES
namespace ass { namespace internal{

using namespace ass::internal;
typedef std::function<bool(const std::string &, AssemblingStatus &, std::size_t&, FileState&)> instFunct_t;

struct HaltF {

    bool operator()(const std::string &line, AssemblingStatus &state, 
                    std::size_t &lineNum, FileState & file);
};

struct AddF {
    bool operator()(const std::string &line, AssemblingStatus &state,
                    std::size_t &lineNum, FileState & file);
};

struct MulF {

    bool operator()(const std::string &line, AssemblingStatus &state, 
                std::size_t &lineNum, FileState & file); 
};

struct SubF{
    bool operator()(const std::string &line, AssemblingStatus &state, 
                        std::size_t &lineNum, FileState & file);

};

struct DivF{

    bool operator()(const std::string &line, AssemblingStatus &state, 
            std::size_t &lineNum, FileState & file);
};


struct AndF {
    bool operator()(const std::string &line, AssemblingStatus &state, 
                std::size_t &lineNum, FileState & file);
};

struct ORF {
    bool operator()(const std::string &line, AssemblingStatus &state, 
                std::size_t &lineNum, FileState & file);
};

struct XORF {
    bool operator()(const std::string &line, AssemblingStatus &state, 
                std::size_t &lineNum, FileState & file);
};

struct NOTF{
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

struct LSWF{
    bool operator()(const std::string &line, AssemblingStatus &state, 
            std::size_t &lineNum, FileState & file);
};

struct AddiF {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};


struct JMPF {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

// unimplemented 
struct LIF {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

struct JALRF {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

struct JALIF {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};



struct RETF {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

struct BEQ {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

struct BNE {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

struct BGT {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

struct BLT {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

struct BGE {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};

struct BLE {
    bool operator()(const std::string &line, AssemblingStatus &state, 
        std::size_t &lineNum, FileState & file);
};


}}