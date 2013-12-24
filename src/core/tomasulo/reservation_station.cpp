#include "../../include/tomasulo/reservation_station.hpp"

reservation_station::reservation_station(FUNCTIONAL_UNIT fu, unsigned int delay){
    static unsigned int counter = 0;
    id = counter ++;
    busy = false;
    this->fu = fu;
    op = operation_type;
	delay_cycles = delay;
    STATE = IDLE;
}

void reservation_station::flush(){
    busy = false;
}

std::array<uint16,3> reservation_station::get_operands(FUNCTIONAL_UNIT fu, Instruction inst){
    uint16_t operand1;
    uint16_t operand2;
    uint16_t operand3;
    switch(fu){
        case FUNCTIONAL_UNIT::ADD:
           operand1 = (uint16_t)inst.get_operand(1);
           operand2 = (uint16_t)inst.get_operand(2);
           t = inst.get_type();
           if(t == OP::ADD || t == OP::SUB){
               operand3 = inst.get_operand(3);
           }
           else if(){
               operand3 = inst.get_operand(3);
           }
           else{
            
           }
           break;
        case FUNCTIONAL_UNIT::LOGICAL:
        case FUNCTIONAL_UNIT::MULL:
            operand1 = (uint16_t)inst.get_operand(1);
            operand2 = (uint16_t)inst.get_operand(2);
            operand3 = (uint16_t)inst.get_operand(3);
            break;
        case FUNCTIONAL_UNIT::LOAD:
            operand1 = (uint16_t)inst.get_operand(1);
            operand2 = (uint16_t)inst.get_operand(2);
            operand3 = (uint16_t)inst.get_immediate();
    }
}


bool reservation_station::issue(Instruction inst, unsigned short rob_id, tomasulo & t){
    if(busy){
        return false;
    }
    state = ISSUED;
    this->rob_id = rob_id;
    busy = true;


    //t.regBoard.used(register);
}

STATE reservation_station::tick(tomasulo & t){

}
