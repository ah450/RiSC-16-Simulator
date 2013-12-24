#include "asm/internal/sym.hpp"



ass::internal::Sym::Sym(cosnt std::string & name, bool global) : defined_(false), global_(global), name_(name) {
    if(global_) {
        file_ = Sym::GLOBAL_FILE;
    }
}


ass::internal::Sym::Sym(const std::string &name, const bfs::path & file,
         addr_t value, SymType type) : defined_(true), global_(false),
    address_(value), type_(type), name_(name), file_(file){}


ass::internal::Sym::Sym(const std::string &name, addr_t value,
     SymType type) : defined_(true), global_(true), address_(value),
     type_(type), name_(name), file_(Sym::GLOBAL_FILE){}
        
void define(const bfs::path & file, addr_t value, SymType type ) {
    if(defined_) {
        throw SymException(std::string("Symbol already defined, Name: ")  + name );
    }
    if(global_) {
        throw SymException(std::string("Attempting to define global sym as local: ") + name );
    }

    file_ = file;
    defined_ = true;
    value_ = value;
    type_ = type;

}
void ass::internal::Sym::define(addr_t value, SymType type ) {
    if(defined_) {
       throw SymException(std::string("Symbol already defined, Name: ")  + name );
    }

    if(!global_) {
       throw SymException(std::string("Attempting to define local sym as global: ") + name ); 
    }

    defined_ = true;
    value_ = value;
    type_ = type;

}