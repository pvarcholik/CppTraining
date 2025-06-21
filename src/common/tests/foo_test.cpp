#include <catch2/catch_all.hpp>
#include "foo.h"

using namespace CppTraining;

SCENARIO("Exercising Foo", "[Foo]")
{
  GIVEN("A Foo with value 42")
  {
    Foo a{42};

    REQUIRE(a.getData() == 42);

    WHEN("Another Foo is copy-constructed from a")
    {
      Foo b{a};

      THEN("They are equal")
      {
        REQUIRE(a == b);
      }

      THEN("Modifying b leaves a unchanged")
      {
        b.setData(100);
        REQUIRE(a.getData() == 42);
        REQUIRE(b.getData() == 100);
        REQUIRE(a != b);
      }
    }

    WHEN("Another Foo is move-constructed from a")
    {
      Foo b = std::move(a);

      THEN("b retains the original value")
      {
        REQUIRE(b.getData() == 42);
      }
    }
  }
}