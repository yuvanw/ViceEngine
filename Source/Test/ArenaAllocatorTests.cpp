#include "catch/catch.hpp"

#include "Memory/ArenaAllocator.h"
#include "Memory/Alignment.h"

TEST_CASE("FArenaAllocator::Allocate") 
{
    const int32 BufferSize = 1024;
    uint8 Buffer[BufferSize];
    FArenaAllocator TestAllocator(&Buffer[0], BufferSize);

    SECTION("Cannot allocate 0 bytes")
    {
        REQUIRE(TestAllocator.Allocate(0) == nullptr);
    }

    SECTION("Cannot allocate size greater than buffer capacity")
    {
        REQUIRE(TestAllocator.Allocate(BufferSize + 1) == nullptr);
    }

    SECTION("Allocations are made linearly across the buffer")
    {
        void* MemoryStart = TestAllocator.Allocate(8);
        REQUIRE(TestAllocator.Allocate(8) == (int8*)MemoryStart + 8);
        REQUIRE(TestAllocator.Allocate(8) == (int8*)MemoryStart + 16);
    }

    SECTION("Allocations are 8-byte aligned by default")
    {
        void* MemoryStart = TestAllocator.Allocate(1);
        REQUIRE(TestAllocator.Allocate(1) == (int8*)MemoryStart + 8);
        REQUIRE(TestAllocator.Allocate(1) == (int8*)MemoryStart + 16);
    }

    SECTION("Allocations can be made using custom alignment")
    {
        void* MemoryStart = TestAllocator.Allocate(1);
        REQUIRE(TestAllocator.Allocate(1, EMemoryAlignment::Four) == (int8*)MemoryStart + 4);
        REQUIRE(TestAllocator.Allocate(1, EMemoryAlignment::Four) == (int8*)MemoryStart + 8);
    }
}

TEST_CASE("FArenaAllocator::Deallocate") 
{
    const int32 BufferSize = 1024;
    uint8 Buffer[BufferSize];
    FArenaAllocator TestAllocator(&Buffer[0], BufferSize);

    SECTION("Deallocation is a no-op")
    {
        void* MemoryStart = TestAllocator.Allocate(8);
        TestAllocator.Deallocate(MemoryStart);
        REQUIRE(TestAllocator.Allocate(8) == (int8*)MemoryStart + 8);
    }
}

TEST_CASE("FArenaAllocator::Clear") 
{
    const int32 BufferSize = 1024;
    uint8 Buffer[BufferSize];
    FArenaAllocator TestAllocator(&Buffer[0], BufferSize);

    SECTION("Clearing the allocator causes subsequent allocations to start from the beginning of the buffer")
    {
        void* MemoryStart = TestAllocator.Allocate(8);
        TestAllocator.Clear();
        REQUIRE(TestAllocator.Allocate(8) == MemoryStart);
    }
}
