#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H
#include <stdint.h>

enum class TYPE {MUL_DIV, ADD_SUB, B_J, LOAD_STORE};
enum class OP{MUL, DIV, ADD, SUB, BRANCH, JUMP, LOAD, STORE};

class reservation_station{

private:
	TYPE type;
	bool busy;
	OP op;
	uint16_t vj;
	uint16_t vk;
	uint16_t qj;
	uint16_t qk;
	unsigned int destination;
	uint16_t address;
	unsigned int delay_cycles;

public:
	reservation_station(std::string type,unsigned int delay);
	virtual uint16_t execute(uint16_t operand1, uint16_t operand2);
};

#endif