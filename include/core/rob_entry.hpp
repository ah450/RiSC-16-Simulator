#ifndef ROB_ENTRY_HPP
#define ROB_ENTRY_HPP
#include "typedefs.hpp"
#include <cstdint>
struct ROB_entry {
    robid_t id;
    address_t dest;
    data_t data;
    address_t extra;
    OP type;
    bool is_memory;
    bool is_ready;
    bool is_jump;
    bool should_jump;
    ROB_entry() : id( 0 ), dest( 0 ), data( 0 ), type( OP::HALT ),
        is_memory( false ),
        is_ready( false ), is_jump( false ), should_jump( false ) {}
};

#endif // ROB_ENTRY_HPP
