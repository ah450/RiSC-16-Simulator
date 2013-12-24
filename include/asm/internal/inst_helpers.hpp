#pragma once
#include "asm/internal/utility.hpp"
#include "asm/internal/internal.hpp"
#include <functional>
#include <memory>
#include <array>

//TODO:GENERATE OPCODES
namespace ass { namespace internal{

using namespace ass::internal;
typedef std::function<bool(const std::string &, AssemblingState &, std::size_t&, FileState&) instFunct_t;

struct HaltF {

    bool operator()(const std::string &line, AssemblingState &state, 
                    std::size_t &lineNum, FileState & file);
};

struct AddF {
    bool operator()(const std::string &line, AssemblingState &state,
                    std::size_t &lineNum, FileState & file);
};

struct MulF {

    bool operator()(const std::string &line, AssemblingState &state, 
                std::size_t &lineNum, FileState & file); 
};

struct SubF{
    bool operator()(const std::string &line, AssemblingState &state, 
                        std::size_t &lineNum, FileState & file);

};

struct DivF{

    bool operator()(const std::string &line, AssemblingState &state, 
            std::size_t &lineNum, FileState & file);
};


struct AndF {
    bool operator()(const std::string &line, AssemblingState &state, 
                std::size_t &lineNum, FileState & file);
};

struct ORF {
    bool operator()(const std::string &line, AssemblingState &state, 
                std::size_t &lineNum, FileState & file);
};

struct XORF {
    bool operator()(const std::string &line, AssemblingState &state, 
                std::size_t &lineNum, FileState & file);
};

struct NOTF{
    bool operator()(const std::string &line, AssemblingState &state, 
        std::size_t &lineNum, FileState & file);
};

struct LSWF{
    bool operator()(const std::string &line, AssemblingState &state, 
            std::size_t &lineNum, FileState & file);
};

struct AddiF {
    bool operator()(const std::string &line, AssemblingState &state, 
        std::size_t &lineNum, FileState & file);
};


}}