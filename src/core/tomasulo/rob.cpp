#include "tomasulo/rob.hpp"


ROB::ROB(unsigned short maxEntries):  rob_entries(), number_of_entries(maxEntries), valid_ids(){

}

void ROB::tick(tomasulo &t) {
    ROB_entry & head = rob_entries.front();
    if(head.is_ready()) {
        // instruction already added all of its execute and WB ticks
        t.notify_instruction_finished();
        if(flush) {
            // handle flushing
            // by clearing entire ROB
            // to flush InstructionBuffer and Reservation Stations
            t.signal_flush(head.data);
            rob_entries.clear();
            // please note that head is INVALID from this point
            valid_ids.reset();
            // all ids can now be reused as ROB is empty
        }else {
            // no flushing
            
            if(head.is_memory){
                // FIXME: Implement when Memory header is available

            }else {
                // don't overwrite R0
                if(head.dest != 0) {
                    
                    t.get_reg_file().regs[head.dest] = head.data;
                }
            }
            // id can now be re-used
            valid_ids.reset(head.id);
            // remove head from ROB
            rob_entries.pop_front(); 
        }

    }
    t.add_ticks(1); // 1 for ROB no matter what.
}



unsigned short ROB::create_entry(){
    if(!is_full()) {
        ROB_entry e;
        bool set = false;
        for(unsigned short i = 0; i < valid_ids.size(); i++) {
            //get first empty id
            if(valid_ids.test(i)) {
                e.id = i;
                set = true;
                break;
            }
        }
        if(set) {
            e.is_ready = e.flush = false;
            rob_entries.push_back(e);
        }else {
            throw ROBException("ROB Had no free ids.");
        }
    }else {
        throw ROBException("Tried to create a new ROB Entry while ROB already full.");
    }
}