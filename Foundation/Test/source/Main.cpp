#include <iostream>
#include <memory>
#include <string>

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

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