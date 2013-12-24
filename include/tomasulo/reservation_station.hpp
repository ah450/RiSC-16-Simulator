#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H
#include <stdint.h>
#include "instruction.hpp"
#include <array>

enum class FUNCTION_UNIT{
    ADD, MULL, LOAD, BRANCH, LOGIC //LI is in ADD FU.
};

enum class STATE {ISSUED, EXECUTED, WRITTEN, IDLE};

class reservation_station{

private:
    const unsigned int id;
    bool busy;
    STATE state;
    FUNCTION_UNIT fu;
    uint16_t vj;
    uint16_t vk;
    unsigned int qj;
    unsigned int qk;
    unsigned int destination;
    uint16_t address;
    unsigned int delay_cycles;
    unsigned short rob_id;

    //member functions
    //used to get operands directly from the instruction
    std::array<uint16_t, 3> get_operands(FUNCTION_UNIT fu, Instruction inst);


public:
    reservation_station(FUNCTION_UNIT fu,
        unsigned int delay);
    bool is_busy(){return busy;}
    bool operator==(reservation_station & rs){return id== rs.id;}
    auto get_id(){return id;}
    void flush();
    STATE tick(tomasulo & t);
    STATE get_state(){return state;}
    bool issue(Instruction inst, unsigned short rob_id, tomasulo & t);
};

#endif
