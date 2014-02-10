#include "core/inst_buffer.hpp"
#include "core/tomasulo.hpp"

static bool isPredicatble( OP );

inst_buffer::inst_buffer( std::size_t size,
                          i_memory *icache ) : MAX_SIZE( size ), fetched( 0 ),
    entries(), icache( icache ), totalDelay(), isFetching( false ),
    flushed( false ), halted( false ), noMoreFetch( false ) {}

void inst_buffer::tick( tomasulo &t ) {


    if ( !halted ) {
        // not done
        if ( !isFetching && ( !noMoreFetch ) ) {

            flushed = false; // don't remember flushed state if not currently fetching

            // fetch new instruction if there is space in inst buffer

            if ( entries.size() < MAX_SIZE ) {
                int delay = 0; // set by memory
                Instruction i( icache->get_data( t.pc, delay ), t.pc );
                entries.push_back( i );
                t.pc += 2;

                // wait delay
                totalDelay = delay;
                totalDelay--; // took one cycle here

                if ( i.type == OP::HALT ) {
                    noMoreFetch = true;
                    halted = true;
                    totalDelay = 0;
                }

                if ( totalDelay ) {
                    // delay was only one cycle. Good job cache !
                    fetched++; // we have one fetched
                } else {
                    // Loading....
                    isFetching = true;
                }

            }

        } else {
            // wait totalDelay
            totalDelay--;

            if ( totalDelay ) {
                // fetching complete
                isFetching = false;

                if ( !flushed ) {
                    // Hasn't been flushed
                    fetched++;
                } else {
                    // Has been flushed
                    flushed = false;
                }
            }
        }
    }

    tick_helper( t );

}

void inst_buffer::flush() {
    // Nothing has been fetched from new address
    fetched = 0;
    // remove all entries if they haven't been issued yet.
    // Its ok because we issue in order. Assuming our implementation works.
    entries.clear();
    // WE HAVE BEEN FLUSHED !!!!
    flushed = true;
    noMoreFetch = false;
    halted = false;
    /****************************************************
     * Note: Should still finish last memory operation. *
     ****************************************************/
}

void inst_buffer::tick_helper( tomasulo &t ) {
    if ( fetched > 0 ) {

        Instruction i = entries.front();

        if ( i.type == OP::HALT ) {
            //FINALLY I HAVE BEEN TICKING FOR AGES
            flush();
            halted = true;
            noMoreFetch = true;
            t.notify_instruction_finished( OP::HALT );
            // WE NOW HAVE ONE LESS FETCHED !!!!!!! ZOMG
            // we do not issue halt instructions
            return;
        } else if ( !t.is_reservation_full( i.type ) && !t.rob.is_full() ) {
            if ( isPredicatble( i.type ) ) {
                // predict and branch
                address_t jumpAddr = i.get_branch_addr();

                if ( jumpAddr != i.pc + 2 ) {
                    flush(); // clears entries
                    t.jumpMe( jumpAddr );
                    reservation_station &rs = t.get_reservation( i.type );
                    auto rob_id = t.rob.create_entry();
                    rs.issue( i, rob_id, t );
                    return;
                }

            }

            // ALWAYS ISSUE NO MATTER WHAT UNLESS ITS HALT !
            reservation_station &rs = t.get_reservation( i.type );
            auto rob_id = t.rob.create_entry();
            rs.issue( i, rob_id, t );
            entries.pop_front(); // remove from buffer
            fetched--; // WE NOW HAVE ONE LESS FETCHED !!!!!!! ZOMG
        }

        // stall on structure hazard
    }
}


static bool isPredicatble( OP t ) {
    return  ( t == OP::BEQ ) | ( t == OP::JALI );
}