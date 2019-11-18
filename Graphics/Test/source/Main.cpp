#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#include <graphics/GraphicsSystem.h>

TEST_CASE("Test main", "[main]")
{
	kern::graphics::GraphicsSystem system;
	system.openWindow(800, 600, "Test");
	REQUIRE(true);
}