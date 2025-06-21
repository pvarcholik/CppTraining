#include <catch2/catch_all.hpp>
#include "vector.h"
#include "foo.h"

using namespace CppTraining;

SCENARIO("Exercising Vector", "[vector]")
{
  GIVEN("An empty Vector<int>")
  {
    Vector<int> vec;

    THEN("It is empty")
    {
      REQUIRE(vec.empty());
      REQUIRE(vec.size() == 0);
      REQUIRE(vec.capacity() == 0);
    }

    WHEN("We reserve space for 10 elements")
    {
      vec.reserve(10);

      THEN("It is still empty but has capacity")
      {
        REQUIRE(vec.empty());
        REQUIRE(vec.size() == 0);
        REQUIRE(vec.capacity() == 10);
      }
    }

    // WHEN("We add an element")
    // {
    //   vec.push_back(42);

    //   THEN("It is no longer empty and has size 1")
    //   {
    //     REQUIRE(!vec.empty());
    //     REQUIRE(vec.size() == 1);
    //     REQUIRE(vec.capacity() >= 1);
    //     REQUIRE(vec[0] == 42);
    //   }
    // }
  }
}