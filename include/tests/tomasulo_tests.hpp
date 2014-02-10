#ifndef TOMASULO_TESTS_H
#define TOMASULO_TESTS_H
#endif

#include "catch.hpp"
#include "./tomasulo/inst_buffer.hpp"
#include "tomasulo/reg_board.hpp"

TEST_CASE( "instruction buffer intialized correctly" ) {

    SECTION( "buffer intialized with max size 3" ) {
        inst_buffer ib0 = inst_buffer( 3 );
        ib0.insert_instruction( 5 );
        ib0.insert_instruction( 287 );
        REQUIRE( ib0.size() == 3 );
    }

    SECTION( "Should allow insertion when contain instructions less the max size" ) {
        inst_buffer ib1 = inst_buffer( 3 );
        ib1.insert_instruction( 5 );
        ib1.insert_instruction( 287 );
        REQUIRE( ib1.insert_instruction( 1924 ) );
    }

    SECTION( "Should not allow more instructions than the max size" ) {
        inst_buffer ib2 = inst_buffer( 3 );
        ib2.insert_instruction( 5 );
        ib2.insert_instruction( 287 );
        ib2.insert_instruction( 2287 );
        REQUIRE( !ib2.insert_instruction( 2287 ) );
    }


}

TEST_CASE( "registers board" ) {

    reg_board rb ;
    SECTION( "check used and un-using register #1" ) {
        REQUIRE( !rb.used( 1 ) );
        rb.record( 1, 200 );
        REQUIRE( rb.used( 1 ) );
        rb.unrecord( 1 );
        REQUIRE( !rb.used( 1 ) );
    }

    reg_board rb1;
    SECTION( "saving and retriveing values" ) {
        rb1.record( 8, 200 );
        REQUIRE( rb1.retrieve( 8 ) == 200 );
    }
}
