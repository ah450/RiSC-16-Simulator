#pragma once
#include "catch.hpp"
#include "../tomasulo/inst_buffer.hpp"

TEST_CASE("instruction buffer intialized correctly"){
	inst_buffer ib = inst_buffer(3);
	SECTION("buffer intialized with max size 3"){
		REQUIRE(ib.size() == 3);
	}
}