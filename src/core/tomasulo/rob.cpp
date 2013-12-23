#include "tomasulo/rob.hpp"


ROB::ROB(unsigned short size):  rob_entries(), MAX_ENTRIES(size){

}

void ROB::tick(tomasulo &t) {
    ROB_entry & head = rob_entries.front();
    if(head.is_ready) {
        // instruction already added all of its execute and WB ticks
        t.notify_instruction_finished();
        if(head.flush) {
            // handle flushing
            // by clearing entire ROB
            // to flush InstructionBuffer and Reservation Stations
            t.signal_flush(head.data);
            rob_entries.clear();
            // please note that head is INVALID from this point
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
            // remove head from ROB
            rob_entries.pop_front(); 
        }

    }
    t.add_ticks(1); // 1 for ROB no matter what.
}



unsigned short ROB::create_entry(){
   if(!is_full()) {
        ROB_entry e;
        e.is_ready=e.flush = false;
        if(rob_entries.empty()) {
            e.id = 0;
        }else {
            e.id = rob_entries.back().id + 1;            
        }
        rob_entries.push_back(e);
    }else {
        throw ROBException("Tried to create a new ROB Entry while ROB already full.");
    }
}

