#include <string>
#include <iostream>
#include <memory>

#define BOOST_TEST_MODULE FoundationTest
#include <boost/test/unit_test.hpp>

#include <foundation/core/TFactory.h>

BOOST_AUTO_TEST_CASE(core_tfactory)
{
	TFactory<std::string, std::string> factory;
	factory.addCreator("foo", [](){
		return new std::string("bar");
	});

	BOOST_CHECK(factory.hasCreator("foo"));
	BOOST_CHECK(!factory.hasCreator("bar"));

	{
		std::shared_ptr<std::string> val{ factory.create("bar") };
		BOOST_CHECK(!val);
	}
	{
		std::shared_ptr<std::string> val{ factory.create("foo") };
		BOOST_ASSERT(val);
		BOOST_CHECK_EQUAL(*val, "bar");
	}
}