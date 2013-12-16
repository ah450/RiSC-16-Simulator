#ifndef CACHE_TEST_H
#define CACHE_TEST_H

#endif // CACHE_TEST_H

#include "catch.hpp"
#include "../../src/core/cache.h"

TEST_CASE("caches initialized correctly","[cache]"){
    cache l_cache = cache(8,2,1);
    l_cache.cache_delay(2);
    l_cache.on_cache_hit(HIT_POLICY::WRITE_BACK);
    l_cache.on_cache_miss(MISS_POLICY::WRITE_ALLOCATE);
    SECTION("correct initalization"){
        REQUIRE(l_cache.run());
    }
    SECTION("cache size not multiple of two"){
        l_cache = cache(6,2,1);
        REQUIRE(!l_cache.run());
    }
    SECTION("cache size too big"){
        l_cache = cache(66666,2,1);
        REQUIRE(!l_cache.run());
    }
    SECTION("line size not even"){
        l_cache = cache(8,3,1);
        REQUIRE(!l_cache.run());
    }
    SECTION("associativity too big"){
        l_cache = cache(8,2,5);
        REQUIRE(!l_cache.run());
    }
    SECTION("delay too small"){
        l_cache.cache_delay(0);
        REQUIRE(!l_cache.run());
    }
}

TEST_CASE("write block","[cache]"){
    cache l_cache = cache(8,2,1);
    l_cache.cache_delay(2);
    l_cache.on_cache_hit(HIT_POLICY::WRITE_BACK);
    l_cache.on_cache_miss(MISS_POLICY::WRITE_ALLOCATE);
    l_cache.run();
    SECTION("write block correct"){
        memory_block block;
        block.data = std::vector<unsigned short>(1,5);
        block.start_address = 0;
        int delay;
        bool b = l_cache.write_block(block,delay);
        REQUIRE(b);
        SECTION("read hit"){
            REQUIRE(l_cache.get_data(0,delay) == 5);
        }
        SECTION("read miss"){
            REQUIRE(l_cache.get_data(2,delay) == 0);
        }
        SECTION("replace"){
            block.data[0] = 4;
            l_cache.write_block(block,delay);
            REQUIRE(l_cache.get_data(0,delay) == 4);
        }
        SECTION("write hit"){
            l_cache.write_data(1,6,delay);
            REQUIRE(l_cache.get_data(1,delay) == 6);
        }
    }
    SECTION("block size not matching"){
        memory_block block;
        block.data = std::vector<unsigned short>(2,5);
        block.start_address = 0;
        int delay;
        REQUIRE(!l_cache.write_block(block,delay));
    }
}

