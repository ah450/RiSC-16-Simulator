#ifndef INST_BUFFER_H
#define INST_BUFFER_H
#include "core/typedefs.hpp"
#include "core/instruction.hpp"
#include "core/i_memory.h"
#include <cstdint>
#include <list>


class tomasulo;

class inst_buffer {
private:
    const std::size_t MAX_SIZE;
    std::size_t fetched;
    std::list<Instruction> entries;
    i_memory *icache;
    delay_t totalDelay;
    bool isFetching, flushed;
    bool halted;
    bool noMoreFetch;
public:
    inst_buffer( std::size_t size, i_memory *icache );
    inst_buffer( const inst_buffer & ) = delete;
    inst_buffer( inst_buffer && ) = delete;
    void tick ( tomasulo &t );
    void flush();
    bool done() {
        return halted;
    }
private:
    void tick_helper( tomasulo &t );
};

#endif
