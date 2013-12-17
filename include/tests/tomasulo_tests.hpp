#ifndef CACHE_TEST_H
#define CACHE_TEST_H

#endif // CACHE_TEST_H

#include "tomasulo/inst_buffer.hpp"

TEST_CASE("instruction buffer intialized correctly"){

	SECTION("buffer intialized with max size 3"){
		inst_buffer ib0 = inst_buffer(3);
		ib0.insert_instruction(5);
		ib0.insert_instruction(287);
		REQUIRE(ib0.size() == 3);
	}

	SECTION("Should allow insertion when contain instructions less the max size"){
		inst_buffer ib1 = inst_buffer(3);
		ib1.insert_instruction(5);
		ib1.insert_instruction(287);
		bool inserted3 = ib1.insert_instruction(2287);
		REQUIRE(inserted3);
	}

	SECTION("Should not allow more instructions than the max size"){
		inst_buffer ib2 = inst_buffer(3);
		ib2.insert_instruction(5);
		ib2.insert_instruction(287);
		ib2.insert_instruction(2287);
		bool inserted4 = ib2.insert_instruction(2287);
		REQUIRE(!inserted4);
	}


}