#include <catch2/catch_test_macros.hpp>

#include <foundation/util/Configuration.h>

TEST_CASE("Parse command line", "[util.Configuration]")
{
	Configuration c;
	const char* a0 = "a=b";
	const char* a1 = "b=1";
	const char* a2 = "c=true";
	const char* argv[3] = { a0, a1, a2 };
	parse(3, argv, c);

	REQUIRE(c.find("a") != c.end());
	REQUIRE((c.at("a") == "b"));

	REQUIRE(c.find("b") != c.end());
	REQUIRE(c.at("b") == 1);

	REQUIRE(c.find("c") != c.end());
	REQUIRE(c.at("c") == true);
}