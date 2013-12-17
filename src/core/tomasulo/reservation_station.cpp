#include "../../include/tomasulo/reservation_station.hpp"

reservation_station::reservation_station(OP operation_type,  uint16_t vj, 
	uint16_t vk, uint16_t qj, uint16_t qk, 
	unsigned int delay, unsigned int address){
	busy = true;
	this->vj = vj;
	this->vk = vk;
	this->qj = qj;
	this->qk = qk;
	op = operation_type;
	delay_cycles = delay;
	this->address = address;

}