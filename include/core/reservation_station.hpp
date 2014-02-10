#ifndef RESERVATION_STATION_H
#define RESERVATION_STATION_H
#include "core/instruction.hpp"
#include "core/typedefs.hpp"
#include <functional>
#include <stdexcept>
#include <cstdint>
#include <array>


class tomasulo;






enum class STATE {
    ISSUED, EXECUTED, WRITTEN, IDLE
};

class reservation_station {

private:
    const stationid_t id;
    STATE state;
    FUNCTION_UNIT fu;
    OP type_of_instruction;
    data_t vj, vk;
    robid_t qj, qk;
    bool qjValid, qkValid;
    address_t pcAtIssue;
    data_t imm;
    delay_t delay_cycles;
    const cycle_t OP_DELAY;
    robid_t rob_id;
    data_t result;
    bool jReady, kReady;
    std::uint8_t rd;
    //member functions
    //used to get operands directly from the instruction
    std::array<uint16_t, 5> get_inst_operands( const Instruction &inst );
    /*used to put value in vx, qx , destination , address
    reg_number is in range [1-7] and index is 1 for vj&qj , 2 for vk&qk */
    void put_reg_or_rob( tomasulo &t, std::uint8_t reg_number, std::uint8_t index );
    void forward( tomasulo &t );
    bool working, busy;
public:
    reservation_station( FUNCTION_UNIT fu,
                         cycle_t delay );
    reservation_station( const reservation_station & ) = delete;
    reservation_station( reservation_station && ) = default;
    bool is_busy() const {
        return busy;
    }
    bool operator==( reservation_station &rs ) {
        return id == rs.id;
    }
    auto get_id() -> decltype( id ) {
        return id;
    }
    void flush();
    STATE tick( tomasulo &t );
    STATE get_state() {
        return state;
    }
    void issue( Instruction inst, robid_t rob_id, tomasulo &t );
    stationid_t get_qj();
    stationid_t get_qk();
    void set_vj( std::uint16_t );
    void set_vk( std::uint16_t );
    void clear_qj();
    void clear_qk();
    STATE tick_MULL( tomasulo &t );
    STATE tick_LOGICAL( tomasulo &t );
    STATE tick_ADD( tomasulo &t );
    STATE tick_BRANCH( tomasulo &t );
    STATE tick_LOAD( tomasulo &t );
    void takeThis( robid_t id, data_t data );
private:
    void issueLW( Instruction &, tomasulo & );
    void issueSW( Instruction &, tomasulo & );
    void issueTernary( Instruction &, tomasulo & );
    void issueNot( Instruction &, tomasulo & );
    void issueBEQ( Instruction &, tomasulo & );
    void issueRet( Instruction &, tomasulo & );
    void issueJmp( Instruction &, tomasulo & );
    void issueJali( Instruction &, tomasulo & );
    void issueJalr( Instruction &, tomasulo & );
    void issueAddi( Instruction &, tomasulo & );
    void issueLI( Instruction &, tomasulo & );
};

#endif
