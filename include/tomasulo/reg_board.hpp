#ifndef REG_BOARD_H
#define REG_BOARD_H
#include <array>
#include <bitset>

class reg_board{
private:
	std::array<unsigned int,7> reg_board_array;
    std::bitset<7> used;
    std::array<unsigned short, 7> producer;

public:
	bool used(unsigned int reg) {
        return used.test(reg);
    }
	bool record(unsigned int reg, unsigned short rob_index){
        if(!used.test(reg)) {
            used.set(reg);
            producer[reg] = rob_index;
            return true;
        }else {
            return false;
        }
    }
	void unrecord(unsigned short rob_index) {
        for(std::size_t i = 0; i < producer.sizse(); i++) {
            if((producer[i] == rob_index) && used.test(i)) {
                used.reset(i);                
            }
        }
    }
	unsigned int retrieve(unsigned int reg){
        return producer[reg];
    }
};

#endif