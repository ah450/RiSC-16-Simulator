#include "asm/internal/sym.hpp"


const ass::internal::bfs::path ass::internal::Sym::GLOBAL_FILE( "GLOBAL" );

ass::internal::Sym::Sym( const std::string &name ) : defined_( false ),
    global_( true ), name_( name ) {

    file_ = Sym::GLOBAL_FILE;

}

ass::internal::Sym::Sym( const std::string &name,
                         const bfs::path &file ) : defined_( false ),
    global_( false ), name_( name ), file_( file ) {

}


ass::internal::Sym::Sym( const std::string &name, const bfs::path &file,
                         addr_t value, SymType type ) : defined_( true ), global_( false ),
    address_( value ), type_( type ), name_( name ), file_( file ) {}


ass::internal::Sym::Sym( const std::string &name, addr_t value,
                         SymType type ) : defined_( true ), global_( true ), address_( value ),
    type_( type ), name_( name ), file_( Sym::GLOBAL_FILE ) {}

void ass::internal::Sym::define( const bfs::path &file, addr_t value,
                                 SymType type ) {
    if ( defined_ ) {
        throw SymException( std::string( "Symbol already defined, Name: " )  + name_ );
    }

    if ( global_ ) {
        throw SymException( std::string( "Attempting to define global sym as local: " )
                            + name_ );
    }

    file_ = file;
    defined_ = true;
    address_ = value;
    type_ = type;

}
void ass::internal::Sym::define( addr_t value, SymType type ) {
    if ( defined_ ) {
        throw SymException( std::string( "Symbol already defined, Name: " )  + name_ );
    }

    if ( !global_ ) {
        throw SymException( std::string( "Attempting to define local sym as global: " )
                            + name_ );
    }

    defined_ = true;
    address_ = value;
    type_ = type;

}