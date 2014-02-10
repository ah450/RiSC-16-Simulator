#ifndef TOMASULO_H
#define TOMASULO_H
#include "core/typedefs.hpp"
#include "core/cache.h"
#include "core/reg_board.hpp"
#include "core/rob.hpp"
#include "core/reservation_station.hpp"
#include "core/instruction.hpp"
#include "core/reg_file.hpp"
#include "core/inst_buffer.hpp"
#include <unordered_map>
#include <cstdint>
#include <memory>
#include <vector>
#include <list>


struct CoreSettings {
    std::size_t robSize, ibuffSize, addUnits,
        mullUnits, loadUnits, branchUnits, logicUnits;
    cycle_t addDelay, mullDelay, loadDelay, branchDelay,
            logicDelay;
    std::shared_ptr<cache> icache, dcache;
    address_t origin;
};

class MemLock {
    bool locked;
    stationid_t id;
public:
    MemLock() : locked( false ) {}
    MemLock( const MemLock & ) = delete;
    MemLock( MemLock && );
    bool lock( stationid_t id ) {
        if ( locked ) {
            return false;
        } else {
            locked = true;
            this->id = id;
            return true;
        }
    }
    void unlock( stationid_t id )  {
        if ( locked ) {
            if ( this->id == id ) {
                locked = false;
            }
        }
    }
};

struct Statistics {
    cycle_t totalInstructions;
    cycle_t totalCycles;
    float addp, mullp, loadp, branchp, logicp;
};

class tomasulo {
private:
    /**********************************
     * Named After FUNCTION_UNIT Enum *
     **********************************/
    std::shared_ptr<i_memory> icache, dcache;
    std::vector<reservation_station> add;
    std::vector<reservation_station> mull;
    std::vector<reservation_station> load;
    std::vector<reservation_station> branch;
    std::vector<reservation_station> logic;
    inst_buffer ibuff;
    cycle_t totalCycles;
    cycle_t totalInstructionExecuted;
    void flush( address_t );
    std::unordered_map<stationid_t, reservation_station *> stationMap;
    std::unordered_map<robid_t, std::list<stationid_t>> waiting;
    MemLock lock;
    class Bus {
        bool used;
    public:
        Bus() : used( false ) {}
        bool tryUse() {
            if ( used ) {
                return false;
            } else {
                used = true;
                return true;
            }
        }
        void reset() {
            used = false;
        }
    } bus;
    std::size_t addc, mullc, loadc, branchc, logicc;
public:
    ROB rob;
    address_t pc;
    std::array<data_t, 8>  regFile;
    reg_board regBoard;
    tomasulo( CoreSettings & );
    tomasulo( const tomasulo & ) = delete;
    tomasulo( tomasulo && ) = delete;
    void jumpMe( address_t loc ) {
        pc = loc;

    }
    void notify_instruction_finished( OP
                                      type ); // So tomasulo/tomasulo can keep track of instructions executed.
    bool is_reservation_full( OP type );
    /**
     * @throws  StructureException if no stations are empty.
     */
    reservation_station &get_reservation( OP type );
    // RESERVATION STATION FORWARDING BUS
    bool signal_bus();

    reservation_station *getRsById( const stationid_t id ) {
        return stationMap.at( id );
    }

    // USE TO FORWARD MAKE SURE YOU HAVE A BUS FIRST OR ALL HELL BREAKS LOOSE.
    void forward( const robid_t robid, const data_t data ) {
        // check if someone is waiting on an robid
        if ( waiting.count( robid ) != 0 )  {
            auto &l = waiting[robid];  // list of reservation station ids

            for ( auto &id : l ) {
                auto rs = getRsById( id ); // get rs pointer
                rs->takeThis( robid, data ); // give it to the rs
            }

            waiting.erase( robid ); // remove all dependencies on this rob entry
        }
    }

    void tick();

    void RegisterWaitForROB( const robid_t robid, const stationid_t id ) {
        waiting[robid].push_back( id );
    }

    void UpdateWaitingForRob( const robid_t robid, data_t data ) {
        if ( waiting.count( robid ) != 0 )  {
            auto &l = waiting[robid];

            for ( auto &id : l ) {
                auto rs = getRsById( id );
                rs->takeThis( robid, data );
            }

            waiting.erase( robid );
        }

        regBoard.unrecord( robid );
    }
    i_memory *memory() {
        return dcache.get();
    }

    bool lockMem( stationid_t id ) {
        return lock.lock( id );
    }

    void unlockMem( stationid_t id ) {
        lock.unlock( id );
    }
    bool done();
    Statistics getStats() {
        return {totalInstructionExecuted , totalCycles, ( float ) addc  / totalInstructionExecuted, ( float ) mullc / totalInstructionExecuted,
                ( float ) loadc / totalInstructionExecuted, ( float ) branchc / totalInstructionExecuted, ( float ) logicc / totalInstructionExecuted
               };
    }
};

#endif // TOMASULO_H
