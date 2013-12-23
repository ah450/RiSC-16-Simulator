#pragma once
#include "asm/internal/typedefs.hpp"
#include <boost/filesystem.hpp>
#include <string>

namespace ass { namespace internal {
    
namespace bfs = boost::filesystem;
enum class SymType {PTR, DATA};

class Sym : std::enable_shared_from_this<Sym>{
    bool defined_, global_;
    addr_t address_;
    SymType type_;
    const std::string name_;
    bfs::path file_;
    std::size_t lineNum_;
public:
    /**
     * @brief Creates an undefined label i.e forward declaration.
     */
    Sym(cosnt std::string & name, bool global) : name_(name), defined_(false), global_(global) {}


    /**
     * @brief Constructs a defined Symbol.
     */
    Sym(bool global, const std::string &name,
        bfs::path file, std::size_t lineNum,  addr_t value, SymType type);


    bool operator==(const Sym &other) const {
        return (file_ == other.file_) && (name_ == other.name_);
    }
    bool operator<(const Sym & other) const {
        return (file_.generic_string() +  name_ ) < (other.file_.generic_string() + other.name_);
    }
    bool defined() { return defined_;}
    const std::string & name() {return name_;}
    bool global() {return global_;}
    void define() {defined_ = true;}
    bfs::path & path(){return file_;}
    std::size_t & lineNumber(){return lineNum_;}
    addr_t & value(){return value_;}

};



}}