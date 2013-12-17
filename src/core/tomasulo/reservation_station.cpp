#include "../../include/tomaulo/reservation_station.hpp"

reservation_station::reservation_station(TYPE typ, OP operation_type, unsigned int delay){
	busy = false;
	vj , vk , qj , qk , destination, address = 0;
	delay_cycles = delay ;
	op = op;
	TYPE = type;
}

uint16_t execute(uint16_t op1, uint16_t op2){
	switch(OP){
		case MUL:
			return op1 * op2;
		case DIV:
			if(op2!=0)return op1 / op2;
		case 
	}
}