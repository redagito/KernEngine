#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <thread>

#include <graphics/graphics/window/CGlfwWindow.h>

TEST_CASE("Test main", "[main]")
{
    CGlfwWindow window;
    window.init(800, 600, "Test");
    auto start = std::chrono::high_resolution_clock::now();
    while (std::chrono::high_resolution_clock::now() - start < std::chrono::seconds(5) && window.isOpen())
    {
        window.processEvents();
        window.swapBuffer();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
	REQUIRE(true);
}