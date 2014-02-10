/**
 * This file will include all test case headers.
 * Eventually includes will be removed and main function will be written here.
 */
#ifdef DEBUG_ME_SOFTLY
#define CATCH_CONFIG_MAIN
#include "tests/catch.hpp"
#include "tests/asm_tests.hpp"
#include "tests/cache_test.hpp"

#else

#include "core/main_memory.h"
#include "core/cache.h"
#include "core/tomasulo.hpp"
#include "asm/assembler.hpp"


#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <stdexcept>
#include <list>


static void getUnits( CoreSettings & );
static void getBuffers( CoreSettings & );
static void setMemory( CoreSettings &,  memory_block & );
static void getCode( CoreSettings &,  memory_block & );
static bool setDefault( CoreSettings & );

static std::shared_ptr<i_memory> makeCache( std::shared_ptr<i_memory>,
        const int level );

std::list<std::shared_ptr<i_memory>> memories;

int main( int argc, char *argv[] ) {
    try {
        CoreSettings sets;
        memory_block memB;

        if ( !setDefault( sets ) ) {
            getUnits( sets );
            getBuffers( sets );
            getCode( sets, memB );
            setMemory( sets, memB );
        }


        tomasulo t( sets );
        std::cout << "\nFinished will now run\n";

        while ( !t.done() ) {
            t.tick();
        }

        Statistics stats = t.getStats();
        std::cout << "\nTotal Instructions: " << stats.totalInstructions
                  << "\nTotal Cycles: " << stats.totalCycles
                  << "\nCPI: " << ( ( float ) stats.totalCycles / stats.totalInstructions )
                  << "\nIPC: " << ( ( float ) stats.totalInstructions / stats.totalCycles )
                  << "\nADD " << stats.addp << "%\nMULL " << stats.mullp
                  << "%\nLOAD " << stats.loadp << "%\nBRANCH " << stats.branchp
                  << "%\nLOGIC " << stats.logicp << "%\n";
        //TODO get miss rates
        std::cout << "\n-- Hit Rates --\n";
        std::cout << "D-CACHE : " <<  memories.front()->get_hit_ratio();
        memories.pop_front();
        std::cout << "\nI-CACHE: "  << memories.front()->get_hit_ratio();
        memories.pop_front();
        auto numCaches = memories.size();

        for ( std::size_t i = 0; i < numCaches; i++ ) {
            std::cout << "\nL" << ( i + 1 ) << "-CACHE: " <<
                      memories.front()->get_hit_ratio();
            memories.pop_front();
        }

        std::cout << '\n';

        return 0;
    } catch ( std::runtime_error &e ) {
        std::cerr << '\n' << e.what() << '\n';
        return -1;
    }

}


static long long readLong( const std::string &prompt ) {
    std::string line;

    while ( true ) {
        std::cout << prompt;
        std::getline( std::cin, line );

        try {

            return std::stoll( line );
        } catch ( ... ) {
            continue;
        }
    }
}


static void getCode( CoreSettings &sets, memory_block &memB ) {

    std::string temp;

    std::vector<boost::filesystem::path> sources;
    std::size_t  fileCount = readLong( "\nPlease enter number of source files: " );

    for ( std::size_t i = 0; i < fileCount; i++ ) {
        std::cout << "\nPlease enter path for file " << ( i + 1 )
                  << ": ";
        std::getline( std::cin, temp );
        sources.emplace_back( temp );
    }

    // OK NOW LET US ASSEMBLE ZE FILES !
    bool OK = ass::assemble( sources, "a.out", std::make_shared<BasicLogger>() );

    if ( !OK ) {
        throw std::runtime_error( "Assembly failure" );
    } else {
        std::ifstream iFile( "a.out", std::ios::binary );

        if ( iFile ) {
            unsigned int size = boost::filesystem::file_size( "a.out" ) / sizeof(
                                    unsigned short );
            memB.data.resize( size - 2 );
            iFile.read( reinterpret_cast<char *>( &( memB.start_address ) ),
                        sizeof( std::uint16_t ) );
            iFile.read( reinterpret_cast<char *>( &( sets.origin ) ),
                        sizeof( std::uint16_t ) );
            iFile.read( reinterpret_cast<char *>( &( memB.data )[0] ),
                        ( size - 2 ) * sizeof( unsigned short ) );

        } else {
            throw std::runtime_error( "Failed to locate a.out" );
        }
    }



}

static void setMemory( CoreSettings &sets, memory_block &memB ) {

    unsigned int mainMemDelay = readLong( "\nPlease enter main memory delay: " );
    std::shared_ptr<main_memory> mainMem = std::make_shared<main_memory>
                                           ( mainMemDelay );
    mainMem->run();
    int delay = 0;
    mainMem->write_block( memB, delay );
    unsigned int dSize = readLong( "\nPlease enter D-CACHE size: " );
    unsigned int dLSize = readLong( "\nPlease enter D-CACHE line size: " );
    unsigned int dAssoc = readLong( "\nPlease enter D-CACHE assoiciativity: " );
    unsigned int dDelay = readLong( "\nPlease enter D-CACHE delay: " );
    HIT_POLICY dhp;
    std::string temp;

    while ( true ) {
        std::cout << "\nChoose hit policy 0 - Write Back 1 - Write Through: ";
        std::getline( std::cin, temp );

        if ( temp[0] == '0' ) {
            dhp = HIT_POLICY::WRITE_BACK;
            break;
        } else if ( temp[0] == '1' ) {
            dhp = HIT_POLICY::WRITE_THROUGH;
            break;
        } else {
            std::cout << "\n please enter 0 or 1";
        }
    }

    MISS_POLICY dmp;

    while ( true ) {
        std::cout << "\nChoose miss policy 0 - Write Allocate 1 - Write Around: ";
        std::getline( std::cin, temp );

        if ( temp[0] == '0' ) {
            dmp = MISS_POLICY::WRITE_ALLOCATE;
            break;
        } else if ( temp[0] == '1' ) {
            dmp = MISS_POLICY::WRITE_AROUND;
            break;
        } else {
            std::cout << "\n please enter 0 or 1";
        }
    }

    unsigned int iSize = readLong( "\nPlease enter I-CACHE size: " );
    unsigned int iLSize = readLong( "\nPlease enter I-CACHE line size: " );
    unsigned int iAssoc = readLong( "\nPlease enter I-CACHE assoiciativity: " );
    unsigned int iDelay = readLong( "\nPlease enter I-CACHE delay: " );
    HIT_POLICY ihp;

    while ( true ) {
        std::cout << "\nChoose hit policy 0 - Write Back 1 - Write Through: ";
        std::getline( std::cin, temp );

        if ( temp[0] == '0' ) {
            ihp = HIT_POLICY::WRITE_BACK;
            break;
        } else if ( temp[0] == '1' ) {
            ihp = HIT_POLICY::WRITE_THROUGH;
            break;
        } else {
            std::cout << "\n please enter 0 or 1";
        }
    }

    MISS_POLICY imp;

    while ( true ) {
        std::cout << "\nChoose miss policy 0 - Write Allocate 1 - Write Around: ";
        std::getline( std::cin, temp );

        if ( temp[0] == '0' ) {
            imp = MISS_POLICY::WRITE_ALLOCATE;
            break;
        } else if ( temp[0] == '1' ) {
            imp = MISS_POLICY::WRITE_AROUND;
            break;
        } else {
            std::cout << "\n please enter 0 or 1";
        }
    }

    //TODO: IMPLEMENT SIZE CONSTRAINT CHECKS
    unsigned int numCaches =
        readLong( "\nPlease enter number of caches other than L1: " );
    std::shared_ptr<i_memory> lower = mainMem;

    for ( int i = numCaches; i > 0; i-- ) {
        // lets make some stuff !
        lower = makeCache( lower, i );
    }

    sets.icache = std::make_shared<cache>( iSize, iLSize, iAssoc );
    sets.icache->on_cache_hit( ihp );
    sets.icache->on_cache_miss( imp );
    sets.icache->cache_delay( iDelay );
    sets.icache->set_lower_memory( lower );
    sets.icache->run();
    sets.dcache = std::make_shared<cache>( dSize, dLSize, dAssoc );
    sets.dcache->on_cache_hit( dhp );
    sets.dcache->on_cache_miss( dmp );
    sets.dcache->cache_delay( dDelay );
    sets.dcache->set_lower_memory( lower );
    sets.dcache->run();
    memories.push_front( sets.icache );
    memories.push_front( sets.dcache );

    std::cout << '\n';

}

static std::shared_ptr<i_memory> makeCache( std::shared_ptr<i_memory> lower,
        const int level ) {

    const std::string header = std::string( "\nPlease enter L" ) + std::to_string(
                                   level ) + "-CACHE";
    unsigned int size = readLong( header + " size: " );
    unsigned int line = readLong( header + " line size: " );
    unsigned int assoc = readLong( header + " assoiciativity: " );
    unsigned int delay = readLong(header +  " delay: " );
    HIT_POLICY hp;
    std::string temp;

    while ( true ) {
    std::cout << "\nChoose hit policy 0 - Write Back 1 - Write Through: ";
    std::getline( std::cin, temp );

        if ( temp[0] == '0' ) {
            hp = HIT_POLICY::WRITE_BACK;
            break;
        } else if ( temp[0] == '1' ) {
            hp = HIT_POLICY::WRITE_THROUGH;
            break;
        } else {
            std::cout << "\n please enter 0 or 1";
        }
    }

    MISS_POLICY mp;

    while ( true ) {
    std::cout << "\nChoose miss policy 0 - Write Allocate 1 - Write Around";
    std::getline( std::cin, temp );

        if ( temp[0] == '0' ) {
            mp = MISS_POLICY::WRITE_ALLOCATE;
            break;
        } else if ( temp[0] == '1' ) {
            mp = MISS_POLICY::WRITE_AROUND;
            break;
        } else {
            std::cout << "\n ENTER 0 OR 1 HOW HARD CAN IT BE ?!";
        }
    }

    std::shared_ptr<cache> c = std::make_shared<cache>( size, line, assoc );
    c->on_cache_miss( mp );
    c->on_cache_hit( hp );
    c->cache_delay( delay );
    c->set_lower_memory( lower );
    c->run();
    memories.push_front( c );
    return c;

}

static void getUnits( CoreSettings &sets ) {


    sets.addUnits = readLong( "\nPlease enter number of ADD/SUB/ADDI units: " );
    sets.addDelay = readLong( "\nPlease enter delay for ADD/SUB/ADDI units: " );
    sets.loadUnits = readLong( "\nPlease enter number of LOAD/STORE units: " );
    sets.loadDelay = readLong( "\nPlease enter delay for LOAD/STORE units: " );
    sets.branchUnits = readLong( "\nPlease enter number of BRANCH/JUMP units: " );
    sets.branchDelay = readLong( "\nPlease enter delay for BRANCH/JUMP units: " );
    sets.mullUnits = readLong( "\nPlease enter number of MULL/DIV units: " );
    sets.mullDelay = readLong( "\nPlease enter delay for MULL/DIV units: " );
    sets.logicUnits = readLong( "\nPlease enter number of Logical units: " );
    sets.logicDelay = readLong( "\nPlease enter delay for Logical units: " );
    std::cout << '\n';

}

static bool setDefault( CoreSettings &sets ) {
    std::string temp;
    std::cout << "\nChoose default settings? yes or no: ";
    std::getline( std::cin, temp );

    if ( temp[0] == 'n' ) {
        return false;
    } else {
        //set units
        sets.addUnits = 2;
        sets.addDelay = 2;
        sets.loadUnits = 1;
        sets.loadDelay = 1;
        sets.branchUnits = 2;
        sets.branchDelay = 1;
        sets.mullUnits = 1;
        sets.mullDelay = 1;
        sets.logicUnits = 2;
        sets.logicDelay = 1;

        //set buffer
        sets.ibuffSize = 10;
        sets.robSize = 10;
        // set memory
        memory_block memB;
        std::vector<boost::filesystem::path> sources;
        sources.emplace_back( "test3.asm" );
        bool OK = ass::assemble( sources, "a.out", std::make_shared<BasicLogger>() );

        if ( !OK ) {
            throw std::runtime_error( "Assembly failure" );
        } else {
            std::ifstream iFile( "a.out", std::ios::binary );

            if ( iFile ) {
                unsigned int size = boost::filesystem::file_size( "a.out" ) / sizeof(
                                        unsigned short );
                memB.data.resize( size - 2 );
                iFile.read( reinterpret_cast<char *>( &( memB.start_address ) ),
                            sizeof( std::uint16_t ) );
                iFile.read( reinterpret_cast<char *>( &( sets.origin ) ),
                            sizeof( std::uint16_t ) );
                iFile.read( reinterpret_cast<char *>( &( memB.data )[0] ),
                            ( size - 2 ) * sizeof( unsigned short ) );

            } else {
                throw std::runtime_error( "Failed to locate a.out" );
            }
        }

        std::shared_ptr<main_memory> mainMem = std::make_shared<main_memory>( 2 );
        mainMem->run();
        int delay = 0;
        mainMem->write_block( memB, delay );
        sets.icache = std::make_shared<cache>( 64, 4, 1 );
        sets.icache->cache_delay( 1 );
        sets.icache->on_cache_miss( MISS_POLICY::WRITE_ALLOCATE );
        sets.icache->on_cache_hit( HIT_POLICY::WRITE_BACK );
        sets.icache->set_lower_memory( mainMem );
        sets.icache->run();
        sets.dcache = std::make_shared<cache>( 64, 4, 1 );
        sets.dcache->cache_delay( 1 );
        sets.dcache->on_cache_miss( MISS_POLICY::WRITE_ALLOCATE );
        sets.dcache->on_cache_hit( HIT_POLICY::WRITE_BACK );
        sets.dcache->set_lower_memory( mainMem );
        sets.dcache->run();
        memories.push_front( sets.icache );
        memories.push_front( sets.dcache );
        return true;
    }
}

static void getBuffers( CoreSettings &sets ) {

    std::string temp;
    sets.ibuffSize = readLong( "Please input Instruction Buffer Size: " );
    sets.robSize = readLong( "\nPlease input Reorder Buffer Size: " );
    std::cout << '\n';

}
#endif