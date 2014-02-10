#include "core/cache.h"
#include <cmath>
#include <vector>
#include <iostream>
#include <ctime>
#include <memory>
#include <cstdint>

#define error(x) {return false;}
#define MINIMU_CACHE_SIZE 2
#define MAXIMUM_CACHE_SIZE 65536
#define ADDRESS_SIZE 16
#define mask(x) (pow(2,x)-1) //mask bits

using ushort = uint16_t;
using uint = uint32_t;

cache::cache( uint cache_size,
              uint line_size,
              uint associativity )
    : cache_size( cache_size ), line_size( line_size )
    , associativity( associativity ), running( false ) {}

void cache::on_cache_hit( HIT_POLICY write_hit ) {
    if ( !running ) {
        this->write_hit = write_hit;
    }
}

void cache::on_cache_miss( MISS_POLICY write_miss ) {
    if ( !running ) {
        this->write_miss = write_miss;
    }
}

void cache::cache_delay( int delay_in_cycles ) {
    if ( !running ) {
        this->delay_in_cycles = delay_in_cycles;
    }
}

void cache::set_lower_memory( std::shared_ptr<i_memory> lower_memory ) {
    this->lower_memory = lower_memory;
}

bool cache::run() {
    if ( !running )
        if ( validate() ) {
            number_of_lines = cache_size / line_size;
            // initialize multidimensional vector
            //warning : should maintain fixed size
            memory_array = std::vector< std::vector<ushort> >
                           ( number_of_lines, std::vector<ushort>
                             ( ( line_size / 2 ) + 2, 0 ) ); // extra two fields for tag
            //, valid and dirty bits

            //set index, tag, offset
            offset_bits = ceil( log2( line_size ) );
            index_bits = ceil( log2( number_of_lines / associativity ) );
            tag_bits = ADDRESS_SIZE - ( offset_bits + index_bits );
            cache_accesses = 0;
            cache_misses = 0;
            this->running = true;
            return true;
        }

    return false;
}

bool cache::validate() {
    if ( cache_size < MINIMU_CACHE_SIZE ) {
        error( "small cache size is too small" );
    }

    if ( cache_size > MAXIMUM_CACHE_SIZE ) {
        error( "cache size is too big" );
    }

    if ( log2( cache_size ) != floor( log2( cache_size ) ) ) {
        error( "cache size is not multiple of 2" );
    }

    if ( line_size < MINIMU_CACHE_SIZE ) {
        error( "line size is too small" );
    }

    if ( cache_size > MAXIMUM_CACHE_SIZE ) {
        error( "line size is too big" );
    }

    if ( ( line_size % 2 ) == 1 ) {
        error( "line size is not even" );
    }

    uint max_associativity = cache_size / line_size;

    if ( associativity < 1 ) {
        error( "associativity is too small" );
    }

    if ( associativity > max_associativity ) {
        error( "associativity is too big" );
    }

    if ( delay_in_cycles < 1 ) {
        error( "delay in cycles is too small" );
    }

    return true;
}

ushort cache::get_data( ushort address, int &delay ) {
    //get index, tag & offset
    ushort offset , index, tag;
    read_address( address, offset,  index,  tag );
    bool found = hit_or_miss( tag, index );
    cache_accesses++;

    if ( found ) { //read hit
        delay += delay_in_cycles;
        return ( memory_array[index][offset / 2] );
    } else {
        cache_misses++;
        delay += delay_in_cycles;
        ushort data = lower_memory->get_data( address,
                                              delay ); //get data from lower mem
        write_block( lower_memory->fetch_block( address, line_size / 2, delay ),
                     delay );//write in memory
        return data;
    }
}

void cache::write_data( unsigned short address,
                        unsigned short data, int &delay ) {
    ushort offset , index, tag;
    read_address( address, offset,  index,  tag );
    bool found = hit_or_miss( tag, index );

    if ( found ) { //write hit
        cache_accesses++;
        delay += delay_in_cycles;

        /*handle dirty*/
        if ( memory_array[index][line_size / 2 + 1] & ( ushort ) 0x2 ) { //block dirty
            /*pass block to lower level*/
            memory_block block;
            block.data = memory_array[index];
            copy( memory_array[index].begin(), memory_array[index].begin() + line_size / 2
                  , block.data.begin() );
            ushort prev_address = 0;
            prev_address |= index >> offset_bits ;
            prev_address |= memory_array[index][line_size / 2]
                            >> ( offset_bits + index_bits );
            block.start_address = prev_address;
            lower_memory->write_block( block, delay );
        }

        memory_array[index][offset / 2] = data;
        memory_array[index][line_size / 2] = tag; //tag

        if ( write_hit == HIT_POLICY::WRITE_THROUGH ) {
            memory_array[index][line_size / 2 + 1] = ( ushort ) 0x1; //valid

            if ( lower_memory ) {
                lower_memory->write_data( address, data, delay );    //update main memory
            }
        } else {
            if ( lower_memory ) {
                memory_array[index][line_size / 2 + 1] = ( ushort ) 0x3;    //valid and dirty
            }
        }
    } else { //write miss
        cache_misses++;

        if ( write_miss == MISS_POLICY::WRITE_ALLOCATE ) {
            /*fetch block then update */
            if ( lower_memory ) {
                write_block( lower_memory->fetch_block
                             ( address, line_size, delay ), delay );
                delay += lower_memory->get_delay();
            }

            write_data( address, data, delay );
        } else {
            cache_accesses++;
            /* update main memory only */
            delay += delay_in_cycles;

            if ( lower_memory ) {
                lower_memory->write_data( address, data, delay );
            }
        }
    }
}


memory_block cache::fetch_block( ushort address, int size, int &delay ) {
    //make sure you have the block
    ushort offset , index, tag;
    read_address( address, offset,  index,  tag );

    if ( ( ushort )size + offset / 2 > ( ushort )line_size /
            2 ) { //it takes multiple blocks
        return buffer_block_fetch( address, size, delay );
    } else { //just one block
        if ( memory_array[index][line_size / 2] == tag
                && ( memory_array[index][line_size / 2 + 1] & ( ushort ) 0x1 ) ) {
            memory_block block;
            block.data = std::vector<ushort>( size, 1 );
            copy( memory_array[index].begin() + offset / 2
                  , memory_array[index].begin() + offset / 2 + size,
                  block.data.begin() );
            block.start_address = address;
            return block;
        } else { //memory doesn't exist
            write_block( lower_memory->fetch_block( address, size, delay ), delay );
            return fetch_block( address, size, delay ); //fetch it from lower level
        }
    }
}


bool cache::write_block( memory_block block, int &delay ) {
    ushort offset , index, tag;
    read_address( block.start_address, offset, index, tag );

    if ( offset % 2 == 1 ) {
        throw 201;
    }

    uint size = block.data.size();

    if ( size + offset / 2 > line_size / 2 ) {
        buffer_block_write( block, delay );
        return true;
    }

    index = get_replacement( block.start_address );

    //handle dirty here
    if ( memory_array[index][line_size / 2 + 1] & ( ushort ) 0x2 ) { //block dirty
        /*pass block to lower level*/
        memory_block block;
        block.data = memory_array[index];
        copy( memory_array[index].begin(), memory_array[index].begin() + line_size / 2
              , block.data.begin() );
        ushort prev_address = 0;
        prev_address |= index >> offset_bits ;
        prev_address |= memory_array[index][line_size / 2]
                        >> ( offset_bits + index_bits );
        block.start_address = prev_address;
        lower_memory->write_block( block, delay );
    }

    memory_array[index] = std::vector<ushort>( line_size / 2 + 2, 0 );
    memory_array[index][line_size / 2] = tag; //set tag
    memory_array[index][line_size / 2 + 1] = ( ushort ) 0x1; //set valid bit
    copy( block.data.begin(), block.data.end(),
          memory_array[index].begin() + offset / 2 );
    return true;
}


ushort cache::get_replacement( ushort address ) { //using Random replacement
    //check valid bits
    ushort offset , index, tag;
    read_address( address, offset, index, tag );

    for ( int i = 0; i < ( short )associativity; ++i ) {
        if ( !( memory_array[index][line_size / 2 + 1] & ( ushort ) 0x1 ) ) {
            return index;
        }

        index += number_of_lines / associativity;
    }//all are valid choose a random element

    index -= number_of_lines / associativity;
    std::srand( time( NULL ) );
    int ind = std::rand() % associativity;
    return index + ind * ( number_of_lines / associativity );
}


bool cache::hit_or_miss( ushort tag, ushort &index ) {
    for ( int i = 0; i < ( short )associativity; ++i ) {
        if ( ( memory_array[index][line_size / 2 + 1] & ( ushort )
                0x1 ) ) { //index is valid
            if ( memory_array[index][line_size / 2] == tag ) { //the same tag
                return true;
            }
        }

        index += number_of_lines / associativity;
    }

    return false;
}

void cache::read_address( ushort address, ushort &offset,
                          ushort &index, ushort &tag ) {

    offset = address & ( ushort )mask( offset_bits ) ;
    index = address & ( ( ushort )mask( index_bits )
                        << offset_bits );
    index = index >> offset_bits;
    tag = address & ( ( ushort )mask( tag_bits )
                      << ( offset_bits + index_bits ) );
    tag = tag >> ( offset_bits + index_bits );
    return;
}



void cache::buffer_block_write( memory_block block, int &delay ) {
    ushort offset , index, tag;
    read_address( block.start_address, offset, index, tag );

    if ( offset % 2 == 1 ) {
        throw 201;    //block starts with byte offset 1
    }

    uint u = line_size / 2 - offset / 2 ; //number of words in first line
    memory_block block2;//first block
    block2.data = std::vector<ushort>( 1, 1 );
    copy( block.data.begin(), block.data.begin() + u, block2.data.begin() );
    block2.start_address = block.start_address;
    write_block( block2, delay );
    ushort address = block.start_address + u * 2;
    uint size = block.data.size();
    int total = ( size - u ) / line_size / 2; //remaining lines

    for ( int i = 0; i < total; ++i ) {
        memory_block block3;
        block.data = std::vector<ushort>( size, 1 );
        copy( block.data.begin() + u + i * line_size / 2,
              block.data.begin() + u + ( i + 1 )*line_size / 2, block3.data.begin() );
        block3.start_address = address;
        address += line_size;//add number of bytes in aline to addres
        write_block( block3, delay );
    }

    if ( ( size - u ) % ( line_size / 2 ) == 0 ) { //last line
        return;
    }

    memory_block block4;
    block4.data = std::vector<ushort>( 1, 1 );
    copy( block.data.begin() + total * line_size / 2, block.data.end()
          , block4.data.begin() );
    block4.start_address = address;
    write_block( block4, delay );
}

memory_block cache::buffer_block_fetch( ushort address, int size, int &delay ) {
    ushort offset , index, tag;
    read_address( address, offset, index, tag );
    memory_block block;
    block.data = std::vector<ushort>( size, 1 );
    block.start_address = address;
    uint u = line_size / 2 - offset / 2 ; //number of words in first line
    uint inc = u;
    memory_block block2 = fetch_block( address, u, delay );
    copy( block2.data.begin(), block2.data.end(), block.data.begin() );
    ushort address2 = address + u * 2;
    int total = ( size - u ) / line_size / 2; //remaining lines

    for ( int i = 0; i < total; ++i ) {
        memory_block block3 = fetch_block( address2, line_size / 2, delay );
        copy( block3.data.begin(), block3.data.end(), block.data.begin() + inc );
        address2 += line_size;//add number of bytes in aline to addres
        inc += line_size / 2;
    }

    if ( ( size - u ) % ( line_size / 2 ) == 0 ) { //last line
        return block;
    }

    memory_block block4 = fetch_block( address2, line_size / 2, delay );
    copy( block4.data.begin(), block4.data.end(), block.data.begin() + inc );
    return block;
}

double cache::get_hit_ratio() {
    if ( cache_accesses == 0 ) {
        return 0.0;
    } else {


        return 1 - ( ( double )cache_misses / ( double )cache_accesses );
    }
}

double cache::get_miss_ratio() {
    if ( cache_accesses == 0 ) {
        return 0.0;
    } else {
        return   ( ( double )cache_misses ) / cache_accesses;
    }
}