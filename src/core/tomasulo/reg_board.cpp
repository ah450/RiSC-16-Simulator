#include "tomasulo/reg_board.hpp"

reg_board::reg_board(){
	for(int i = 0; i<8; i++){
		reg_board_array[i] = 0;
	}
}

bool reg_board::used(unsigned int index){
	if(reg_board_array[index-1] == 0)
		return false;
	else
		return true;
}

void reg_board::record(unsigned int index, unsigned int rob_index){
	reg_board_array[index-1] = rob_index; 
}

void reg_board::unrecord(unsigned int index){
	reg_board_array[index-1] = 0;
}

unsigned int reg_board::retrieve(unsigned int index){
	return reg_board_array[index-1]; 
}