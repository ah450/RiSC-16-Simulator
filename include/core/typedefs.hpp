#pragma once
#include <cstdint>

typedef std::int16_t data_t;
typedef std::uint16_t address_t;
typedef std::size_t cycle_t;
typedef std::uint16_t robid_t;
typedef std::uint16_t stationid_t;

enum class FUNCTION_UNIT {
    ADD, MULL, LOAD, BRANCH, LOGIC //LI is in ADD FU.
};


// 23 operations for 23 different types of instructions
enum class OP : std::size_t {
    LW, SW, ADDI, JMP, LI, JALR, JALI, ADD, SUB,
    MUL, DIV, HALT, AND, OR, XOR, NOT, RET, BEQ
};

class delay_t {
private:
    cycle_t counter;
public:
    delay_t( cycle_t total = 0 ) : counter( total ) {}
    operator bool() const {
        return counter == 0;
    }
    bool operator!() const {
        return counter != 0;
    }
    delay_t &operator=( cycle_t counter ) {
        this->counter = counter;
        return *this;
    }
    // post decrement
    delay_t operator--( int ) {
        delay_t toReturn( *this );

        if ( counter > 1 ) {
            counter -= 1;
        } else {
            counter = 0;
        }

        return toReturn;
    }

    // pre decrement
    delay_t &operator--() {
        if ( counter > 1 ) {
            counter -= 1;
        } else {
            counter = 0;
        }

        return *this;
    }

    delay_t &operator-=( cycle_t offset ) {
        if ( counter > offset ) {
            counter -= offset;
        } else {
            counter = 0;
        }

        return *this;
    }
};