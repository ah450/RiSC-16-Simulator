#pragma once
#include "core/rob_entry.hpp"
#include "core/typedefs.hpp"
#include "core/exceptions.hpp"
#include "core/i_memory.h"
#include <algorithm>
#include <stdexcept>
#include <functional>
#include <cstdint>
#include <string>
#include <list>



class tomasulo;


/*******************************************************
 * @brief This class Represents the Re-order Buffer.   *
 * @details Maximum size is 2^16 -1 i.e unsigned short.*
 * This is to gaurantee correct ids for ROB entries.   *
 *******************************************************/
class ROB {
private:
    std::list<ROB_entry> rob_entries;
    const unsigned short MAX_ENTRIES;
    const std::function<void ( address_t )> flushFunct;
    i_memory *dcache;
    bool working;
    delay_t timeToWait;
    void newWork( tomasulo &t );
public:
    /****************************************
     * @param MAX_ENTRIES size of ROB.*
     ****************************************/
    ROB( const unsigned short size, i_memory *dcache,
         std::function<void ( address_t )> flushFunct )
        : rob_entries(),  MAX_ENTRIES( size ),  flushFunct( flushFunct ),
          dcache( dcache ),
          working( false ) {}
    ROB( ROB && ) = delete;
    ROB( const ROB & ) = delete;
    auto getEntry( unsigned short id ) -> decltype( rob_entries.end() ) {
        return std::find_if( rob_entries.begin(), rob_entries.end(),
        [&id]( ROB_entry & e ) {
            return e.id == id;
        } );
    }

    bool is_full() {
        return rob_entries.size() >= MAX_ENTRIES;
    }

    void tick( tomasulo & );

    /***************************************************
     * @brief Creates a new ROB Entry with a unique ID.*
     * @throws ROBException if ROB is full.            *
     * @return ID of newly created entry.              *
     ***************************************************/
    robid_t create_entry();
    bool done() {
        return rob_entries.empty() && ( !working );
    }
};

