#include <catch2/catch_test_macros.hpp>

#include <foundation/util/CEightCC.h>

TEST_CASE("Default contructor sets to zero", "[util.CEightCC]")
{
	CEightCC e;
	REQUIRE(e.get() == 0);
}