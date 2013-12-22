#include "tomasulo/rob.hpp"


ROB::ROB(unsigned short maxEntries):  rob_entries(), number_of_entries(maxEntries), valid_ids(){

}

void ROB::tick(tomasulo &) {

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