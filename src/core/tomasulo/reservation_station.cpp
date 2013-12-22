#include "../../include/tomasulo/reservation_station.hpp"

reservation_station::reservation_station(OP operation_type, unsigned int delay){
    static unsigned int counter = 0;
    id = counter ++;
    busy = false;
    op = operation_type;
	delay_cycles = delay;
}
