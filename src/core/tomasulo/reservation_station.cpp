#include "../../include/tomasulo/reservation_station.hpp"

reservation_station::reservation_station(OP operation_type, unsigned int delay){
    static unsigned int counter = 0;
    id = counter ++;
    busy = false;
    op = operation_type;
	delay_cycles = delay;
}

void reservation_station::flush(){
    busy = false;
}

bool reservation_station::issue(Instruction inst, unsigned short rob_id){
    if(busy){
        return false;
    }
    state = ISSUED;
    

}

//to decode the type of functional unit from instruction
FUNCTIONAL_UNIT get_functional_unit(Instruction inst){
    if()
}

STATE reservation_station::tick(tomasulo & t){

}
