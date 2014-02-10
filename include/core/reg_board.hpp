#pragma once
#include <array>
#include <bitset>

class reg_board {
private:
    std::bitset<8> used_;
    std::array<unsigned short, 8> producer;


public:
    bool used( const unsigned int reg ) {
        return used_.test( reg );
    }
    void record( const unsigned int reg, const robid_t rob_index ) {
        used_.set( reg );
        producer[reg] = rob_index;
    }
    void unrecord( const robid_t rob_index ) {
        for ( std::size_t i = 0; i < producer.size(); i++ ) {
            if ( ( producer[i] == rob_index ) && used_.test( i ) ) {
                used_.reset( i );
            }
        }
    }
    robid_t retrieve( const robid_t reg ) {
        return producer[reg];
    }
    void flush() {
        used_.reset();
    }
};
