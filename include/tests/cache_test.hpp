#ifndef CACHE_TEST_H
#define CACHE_TEST_H

#endif // CACHE_TEST_H

#include "catch.hpp"
#include "../../src/core/cache.h"

#include <memory>

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

TEST_CASE("single level cache","[cache]"){
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
        SECTION("write hit"){
            l_cache.write_data(1,6,delay);
            REQUIRE(l_cache.get_data(1,delay) == 6);
        }
    }
}

TEST_CASE("associativity","[cache]"){
    cache l_cache = cache(8,2,2);
    l_cache.cache_delay(2);
    l_cache.on_cache_hit(HIT_POLICY::WRITE_BACK);
    l_cache.on_cache_miss(MISS_POLICY::WRITE_ALLOCATE);
    l_cache.run();
    memory_block block;
    block.data = std::vector<unsigned short>(1,5);
    block.start_address = 0;
    int delay;
    l_cache.write_block(block,delay);
    block.data = std::vector<unsigned short>(1,4);
    block.start_address = 16;
    l_cache.write_block(block,delay);
    REQUIRE(l_cache.get_data((ushort)0,delay) == 5);
    REQUIRE(l_cache.get_data((ushort)16,delay) == 4);
}

TEST_CASE("multilevel cache same line size","[cache]"){
    cache l_cache = cache(8,2,1);
    l_cache.cache_delay(2);
    l_cache.on_cache_hit(HIT_POLICY::WRITE_BACK);
    l_cache.on_cache_miss(MISS_POLICY::WRITE_ALLOCATE);
    cache l_cache2 = cache(16,2,1);
    l_cache2.cache_delay(2);
    l_cache2.on_cache_hit(HIT_POLICY::WRITE_BACK);
    l_cache2.on_cache_miss(MISS_POLICY::WRITE_ALLOCATE);

    l_cache.run();
    l_cache2.run();

    auto l2_cache = std::make_shared<cache>(l_cache2);
    l_cache.set_lower_memory(l2_cache);
    memory_block block;
    block.data = std::vector<unsigned short>(1,5);
    block.start_address = 0;
    int delay = 0;
    REQUIRE(l2_cache->write_block(block,delay));
    REQUIRE(l_cache.get_data(0,delay) == 5);//l1 miss l2 hit

    l_cache.write_data((ushort)0,(ushort)8,delay);//dirty block
    l_cache.write_data((ushort)0,(ushort)7,delay);//block pushed to lower level
    REQUIRE(l2_cache->get_data((ushort)0,delay) == 8);

    block.start_address = 2;
    l2_cache->write_block(block,delay);
    l_cache.write_data((ushort)2, (ushort)2,delay);//write miss fetch
    REQUIRE(l2_cache->get_data((ushort)2,delay) == 5);
}

TEST_CASE("multilevel cache write through","[cache]"){
    cache l_cache = cache(8,2,1);
    l_cache.cache_delay(2);
    l_cache.on_cache_hit(HIT_POLICY::WRITE_THROUGH);
    l_cache.on_cache_miss(MISS_POLICY::WRITE_AROUND);
    cache l_cache2 = cache(16,2,1);
    l_cache2.cache_delay(2);
    l_cache2.on_cache_hit(HIT_POLICY::WRITE_THROUGH);
    l_cache2.on_cache_miss(MISS_POLICY::WRITE_AROUND);

    l_cache.run();
    l_cache2.run();

    auto l2_cache = std::make_shared<cache>(l_cache2);
    l_cache.set_lower_memory(l2_cache);
    memory_block block;
    block.data = std::vector<unsigned short>(1,5);
    block.start_address = 0;
    int delay = 0;
    REQUIRE(l2_cache->write_block(block,delay));
    REQUIRE(l_cache.get_data(0,delay) == 5);//l1 miss l2 hit

    l_cache.write_data((ushort)0,(ushort)8,delay);//update lower memory too
    REQUIRE(l2_cache->get_data((ushort)0,delay) == 8);

    block.start_address = 2;
    l2_cache->write_block(block,delay);
    l_cache.write_data((ushort)2, (ushort)2,delay);//write miss dont fetch
    REQUIRE(l2_cache->get_data((ushort)2,delay) == 2);
}

TEST_CASE("multilevel cache smaller bigger line size"){
    cache l_cache = cache(8,2,1);
    l_cache.cache_delay(2);
    l_cache.on_cache_hit(HIT_POLICY::WRITE_BACK);
    l_cache.on_cache_miss(MISS_POLICY::WRITE_ALLOCATE);
    cache l_cache2 = cache(16,4,1);
    l_cache2.cache_delay(2);
    l_cache2.on_cache_hit(HIT_POLICY::WRITE_BACK);
    l_cache2.on_cache_miss(MISS_POLICY::WRITE_ALLOCATE);

    l_cache.run();
    l_cache2.run();

    auto l2_cache = std::make_shared<cache>(l_cache2);
    l_cache.set_lower_memory(l2_cache);
    memory_block block;
    block.data = std::vector<unsigned short>(2,5);
    block.start_address = 0;
    int delay = 0;
    REQUIRE(l2_cache->write_block(block,delay));
    REQUIRE(l_cache.get_data(0,delay) == 5);//l1 miss l2 hit

    l_cache.write_data((ushort)0,(ushort)8,delay);//dirty block
    l_cache.write_data((ushort)0,(ushort)7,delay);//block pushed to lower level
    REQUIRE(l2_cache->get_data((ushort)0,delay) == 8);

    block.start_address = 4;
    l2_cache->write_block(block,delay);
    l_cache.write_data((ushort)4, (ushort)2,delay);//write miss fetch
    REQUIRE(l2_cache->get_data((ushort)4,delay) == 5);
}

TEST_CASE("multilevel cache bigger smaller line size"){
    cache l_cache = cache(8,4,1);
    l_cache.cache_delay(2);
    l_cache.on_cache_hit(HIT_POLICY::WRITE_BACK);
    l_cache.on_cache_miss(MISS_POLICY::WRITE_ALLOCATE);
    cache l_cache2 = cache(16,2,1);
    l_cache2.cache_delay(2);
    l_cache2.on_cache_hit(HIT_POLICY::WRITE_BACK);
    l_cache2.on_cache_miss(MISS_POLICY::WRITE_ALLOCATE);

    l_cache.run();
    l_cache2.run();

    auto l2_cache = std::make_shared<cache>(l_cache2);
    l_cache.set_lower_memory(l2_cache);
    memory_block block;
    block.data = std::vector<unsigned short>(1,5);
    block.start_address = 0;
    int delay = 0;
    REQUIRE(l2_cache->write_block(block,delay));
    block.start_address = 2;
    l2_cache->write_block(block,delay);
    REQUIRE(l_cache.get_data(2,delay) == 5);//l1 miss l2 hit

    l_cache.write_data((ushort)0,(ushort)8,delay);//dirty block
    l_cache.write_data((ushort)2,(ushort)7,delay);//block pushed to lower level
    REQUIRE(l2_cache->get_data((ushort)0,delay) == 8);

    block.start_address = 4;
    l2_cache->write_block(block,delay);
    l_cache.write_data((ushort)4, (ushort)2,delay);//write miss fetch
    REQUIRE(l2_cache->get_data((ushort)4,delay) == 5);
}
