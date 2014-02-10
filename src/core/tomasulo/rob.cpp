#include "core/rob.hpp"
#include "core/tomasulo.hpp"


void ROB::newWork( tomasulo &t ) {
    if ( !rob_entries.empty() ) {

        ROB_entry &head = rob_entries.front();

        if ( head.is_ready ) {
            // instruction already added all of its execute and WB ticks

            if ( head.is_memory ) {
                //memory commits
                if ( t.lockMem( 0 ) ) {

                    int delay = 0;
                    dcache->write_data( head.dest, head.data, delay );
                    timeToWait = delay;
                    timeToWait--; // current cycle

                    if ( !timeToWait ) {
                        working = true;
                    }

                    t.notify_instruction_finished( head.type );
                }
            } else if ( head.is_jump ) {
                // jump instructions
                t.notify_instruction_finished( head.type );

                if ( head.should_jump ) {
                    // uncoditional jumps or wrongly predicted branches

                    if ( head.type == OP::BEQ ) {
                        // should also flush
                        flushFunct( head.dest );
                        t.jumpMe( head.dest ); // GO GO GADGET !
                        rob_entries.clear();
                    } else if ( head.type == OP::JALR ) {
                        t.jumpMe( head.dest );
                        flushFunct( head.dest );

                        if ( head.extra != 0 ) {
                            // protect zero
                            t.regFile[head.extra] = head.data;
                        }

                        // JALR can not be predicted
                        t.UpdateWaitingForRob( head.id, head.data );
                        rob_entries.clear();
                    } else if ( head.type == OP::JALI ) {
                        // JALI is always predicted right
                        // do nothing except commit link register
                        if ( head.extra != 0 ) {
                            // protect zero
                            t.regFile[head.extra] = head.data;
                        }

                        t.UpdateWaitingForRob( head.id, head.data );
                        // remove entry
                        rob_entries.pop_front();
                    } else if ( head.type == OP::RET ) {
                        //RET can not be predicted
                        // must flush
                        flushFunct( head.dest );
                        t.jumpMe( head.dest );
                        rob_entries.clear();
                    } else if ( head.type == OP::JMP ) {
                        flushFunct( head.dest );
                        t.jumpMe( head.dest );
                        rob_entries.clear();
                    }

                } else {
                    // pop a correctly predicted BEQ
                    rob_entries.pop_front();
                }

            } else {
                // register commits from arithmetic and logic
                // don't overwrite R0
                if ( head.dest != 0 ) {

                    t.regFile[head.dest] = head.data;
                }

                // now lets update all station waiting on this entry !!!!!!!!
                t.UpdateWaitingForRob( head.id, head.data );
                rob_entries.pop_front();
                t.notify_instruction_finished( head.type );
            }

        }

        // head not ready
    }

    // list is empty
}




void ROB::tick( tomasulo &t ) {

    if ( !working ) {
        newWork( t );
    } else {
        timeToWait--; // this cycle

        if ( timeToWait ) {
            working = false;
            t.unlockMem( 0 );
        }
    }
}



unsigned short ROB::create_entry() {
    if ( !is_full() ) {
        ROB_entry e;

        if ( rob_entries.empty() ) {
            e.id = 0;
        } else {
            e.id = rob_entries.back().id + 1;
        }

        rob_entries.push_back( e );
        return e.id;
    } else {
        throw ROBException( "Tried to create a new ROB Entry while ROB already full." );
    }
}

