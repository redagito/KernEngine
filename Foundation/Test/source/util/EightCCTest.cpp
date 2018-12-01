#include <catch.hpp>

#include <foundation/util/EightCC.h>

TEST_CASE("Default contructor sets to zero", "[util.EightCC]")
{
	EightCC e;
	REQUIRE(e.get() == 0);
}