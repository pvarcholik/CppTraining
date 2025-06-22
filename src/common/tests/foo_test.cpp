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

    WHEN("Another Foo is copy-assigned from a")
    {
      Foo b;
      b = a;

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

    WHEN("Another Foo is move-assigned from a")
    {
      Foo b{100};
      b = std::move(a);

      THEN("b retains the a's value")
      {
        REQUIRE(b.getData() == 42);
      }
    }

    WHEN("Converting a to a string")
    {
      THEN("The string representation is correct")
      {
        REQUIRE(to_string(a) == "42");
      }
    }
  }
}

TEST_CASE("Explicit destruction of Foo")
{
    Foo* f = new Foo{42};
    delete f;
}