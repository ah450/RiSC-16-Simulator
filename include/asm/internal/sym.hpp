#pragma once
#include "asm/internal/typedefs.hpp"
#include <boost/filesystem.hpp>
#include <stdexcept>
#include <string>

namespace ass { namespace internal {
    
namespace bfs = boost::filesystem;
enum class SymType {PTR, DATA};

struct SymException : std::runtime_error {
    SymException(const std::string &what): std::runtime_error(what){}
    SymException(const SymException &) = default;
    SymException(SymException &&) = default;
};

class Sym : std::enable_shared_from_this<Sym>{
    bool defined_, global_;
    addr_t address_;
    SymType type_;
    const std::string name_;
    bfs::path file_;
    static const bfs::path GLOBAL_FILE;
public:
    /**
     * @brief Creates an undefined label i.e forward declaration.
     */
    Sym(const std::string & name, bool global);


    /**
     * @brief Constructs a defined Symbol non global.
     */
    Sym(const std::string &name,
        const bfs::path & file, addr_t value, SymType type);
    /**
     * @brief Constructs a defined global Symbol.
     */
    Sym(const std::string &name, addr_t value, SymType type);

    Sym(const Sym &) = delete;
    Sym(Sym &&) = delete;
    Sym & operator=(const Sym&) = delete;
    Sym & operator=(Sym &&) = delete;

    bool operator==(const Sym &other) const {
        return (file_ == other.file_) && (name_ == other.name_);
    }
    bool operator<(const Sym & other) const {
        return (file_.generic_string() +  name_ ) < (other.file_.generic_string() + other.name_);
    }

    bool defined() const { return defined_;}
    const std::string & name() const {return name_;}
    bool global() const {return global_;}
    const bfs::path & path() const {return file_;}
    const addr_t & value(){return address_;}
    /**
     * @throws SymException if already defined
     * @throws SymException if it is a global sym
     */
    void define(const bfs::path & file, addr_t value, SymType type );
    /**
     * @throws  SymException if already defined
     */
    void define(addr_t value, SymType type );
};



}}