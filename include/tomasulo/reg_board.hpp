#ifndef REG_BOARD_H
#define REG_BOARD_H
#include <array>

class reg_board{
private:
	std::array<unsigned int,7> reg_board_array;

public:
	reg_board();
	virtual bool used(unsigned int index);
	virtual void record(unsigned int index, unsigned int rob_index);
	virtual void unrecord(unsigned int rob_index);
	virtual unsigned int retrieve(unsigned int index);
};

#endif