#include "core/tomasulo.hpp"
#include "core/utility.hpp"
#include <algorithm>
#include <iterator>
#include <iostream>

/****************************
 * Helper Static functions. *
 ****************************/
static bool hasEmpty( const std::vector<reservation_station> & );

tomasulo::tomasulo( CoreSettings &settings ) : icache( settings.icache ),
    dcache( settings.dcache ),
    ibuff( settings.ibuffSize, settings.icache.get() ), totalCycles( 0 ),
    totalInstructionExecuted( 0 ), stationMap(), rob( settings.robSize,
            settings.dcache.get(), std::bind( &tomasulo::flush, this,
                    std::placeholders::_1 ) ),
    pc( settings.origin ), addc( 0 ), mullc( 0 ), loadc( 0 ), branchc( 0 ),
    logicc( 0 ) {

    for ( std::size_t i = 0; i < settings.addUnits; i++ ) {
        add.emplace_back( FUNCTION_UNIT::ADD, settings.addDelay );
    }

    for ( std::size_t i = 0; i < settings.mullUnits; i++ ) {
        mull.emplace_back( FUNCTION_UNIT::MULL, settings.mullDelay );
    }

    for ( std::size_t i = 0; i < settings.loadUnits; i++ ) {
        load.emplace_back( FUNCTION_UNIT::LOAD, settings.loadDelay );
    }

    for ( std::size_t i = 0; i < settings.branchUnits; i++ ) {
        branch.emplace_back( FUNCTION_UNIT::BRANCH, settings.branchDelay );
    }

    for ( std::size_t i = 0; i < settings.logicUnits; i++ ) {
        logic.emplace_back( FUNCTION_UNIT::LOGIC, settings.logicDelay );
    }

    auto func = [this] ( reservation_station & rs ) {
        stationMap[rs.get_id()] = &rs;
    };
    std::for_each( add.begin(), add.end(), func );
    std::for_each( mull.begin(), mull.end(), func );
    std::for_each( load.begin(), load.end(), func );
    std::for_each( branch.begin(), branch.end(), func );
    std::for_each( logic.begin(), logic.end(), func );
}






void tomasulo::flush( std::uint16_t address ) {
    pc = address;
    ibuff.flush();
    auto rsFunc = []( reservation_station & rs ) {
        rs.flush();
    };
    std::for_each( add.begin(), add.end(), rsFunc );
    std::for_each( mull.begin(), mull.end(), rsFunc );
    std::for_each( load.begin(), load.end(), rsFunc );
    std::for_each( branch.begin(), branch.end(), rsFunc );
    std::for_each( logic.begin(), logic.end(), rsFunc );
    regBoard.flush();
}

void tomasulo::notify_instruction_finished( OP type ) {
    totalInstructionExecuted++;
    FUNCTION_UNIT fu = OP_MAPING.at( type );

    switch ( fu ) {
        case FUNCTION_UNIT::ADD:
            addc++;
            break;

        case FUNCTION_UNIT::MULL:
            mullc++;
            break;

        case FUNCTION_UNIT::LOAD:
            loadc++;
            break;

        case FUNCTION_UNIT::BRANCH:
            branchc++;
            break;

        case FUNCTION_UNIT::LOGIC:
            logicc++;
            break;
    }
}


bool tomasulo::is_reservation_full( OP type ) {
    FUNCTION_UNIT fu = OP_MAPING.at( type );

    switch ( fu ) {
        case FUNCTION_UNIT::ADD:
            return !hasEmpty( add );

        case FUNCTION_UNIT::MULL:
            return !hasEmpty( mull );

        case FUNCTION_UNIT::LOAD:
            return !hasEmpty( load );

        case FUNCTION_UNIT::BRANCH:
            return !hasEmpty( branch );

        case FUNCTION_UNIT::LOGIC:
            return !hasEmpty( logic );

        default:
            return false;
    }

}

reservation_station &tomasulo::get_reservation( OP type ) {
    FUNCTION_UNIT fu = OP_MAPING.at( type );
    std::vector<reservation_station>::iterator rs;
    auto funct = []( const reservation_station & rs ) {
        return !rs.is_busy();
    };

    switch ( fu ) {
        case FUNCTION_UNIT::ADD:
            rs = std::find_if( add.begin(), add.end(), funct );

            if ( rs != add.end() ) {
                return *rs;
            }

        case FUNCTION_UNIT::MULL:
            rs = std::find_if( mull.begin(), mull.end(), funct );

            if ( rs != mull.end() ) {
                return *rs;
            }

        case FUNCTION_UNIT::LOAD:
            rs = std::find_if( load.begin(), load.end(), funct );

            if ( rs != load.end() ) {
                return *rs;
            }

        case FUNCTION_UNIT::BRANCH:
            rs = std::find_if( branch.begin(), branch.end(), funct );

            if ( rs != branch.end() ) {
                return *rs;
            }

        case FUNCTION_UNIT::LOGIC:
            rs = std::find_if( logic.begin(), logic.end(), funct );

            if ( rs != logic.end() ) {
                return *rs;
            }
    }

    throw StructureException( "No empty reservation station" );
}

bool tomasulo::signal_bus() {
    return bus.tryUse();
}
static bool hasEmpty( const std::vector<reservation_station> &stations ) {
    for ( auto &rs : stations ) {
        if ( !rs.is_busy() ) {
            return true;
        }
    }

    return false;
}

void tomasulo::tick() {
    if ( !done() ) {


        ibuff.tick( *this );
        auto rsFunc = [this] ( reservation_station & rs ) {
            rs.tick( *this );
        };
        // TICK EM ALL !
        std::for_each( add.begin(), add.end(), rsFunc );
        std::for_each( mull.begin(), mull.end(), rsFunc );
        std::for_each( load.begin(), load.end(), rsFunc );
        std::for_each( branch.begin(), branch.end(), rsFunc );
        std::for_each( logic.begin(), logic.end(), rsFunc );
        rob.tick( *this );
        bus.reset(); // can now be re-used
        totalCycles++; // THIS IS A CYCLE !
    }
}

bool tomasulo::done() {
    return ibuff.done() && rob.done();
}