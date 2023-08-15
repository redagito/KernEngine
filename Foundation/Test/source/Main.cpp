#include <iostream>
#include <memory>
#include <string>

#include <catch2/catch_test_macros.hpp>

#include <foundation/core/TFactory.h>

TEST_CASE("Add creator to factory", "[core.TFactory]")
{
	TFactory<std::string, std::string> factory;
	factory.addCreator("foo", []() { return new std::string("bar"); });

	REQUIRE(factory.hasCreator("foo"));
	REQUIRE_FALSE(factory.hasCreator("bar"));
	REQUIRE_FALSE(factory.create("bar"));

	{
		auto val{ factory.create("foo") };
		REQUIRE(val);
		REQUIRE(*val == "bar");
	}
}