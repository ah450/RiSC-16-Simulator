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
