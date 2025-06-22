#include <catch2/catch_all.hpp>
#include "vector.h"
#include "foo.h"

using namespace CppTraining;

SCENARIO("Exercising vector", "[vector]")
{
  GIVEN("An empty vector<Foo>")
  {
    const Foo a{1};
    const Foo b{2};
    const Foo c{3};

    Vector<Foo> values;

    THEN("It is empty")
    {
      REQUIRE(values.empty());
      REQUIRE(values.size() == 0_z);
      REQUIRE(values.capacity() == 0_z);
    }

    WHEN("We reserve space for 10 elements")
    {
      values.reserve(10_z);

      THEN("It is still empty but has capacity")
      {
        REQUIRE(values.empty());
        REQUIRE(values.size() == 0_z);
        REQUIRE(values.capacity() == 10_z);
      }
    }

    WHEN("We resize it to 5 elements")
    {
      values.resize(5_z);

      THEN("It is no longer empty and has size 5")
      {
        REQUIRE(!values.empty());
        REQUIRE(values.size() == 5_z);
        REQUIRE(values.capacity() >= 5_z);
      }
    }

    WHEN("We clear it")
    {
      values.clear();

      THEN("It is empty again")
      {
        REQUIRE(values.empty());
        REQUIRE(values.size() == 0_z);
        REQUIRE(values.capacity() >= 0_z);
      }
    }

    WHEN("We shrink it to fit")
    {
      values.shrink_to_fit();

      THEN("It is empty and capacity is zero")
      {
        REQUIRE(values.empty());
        REQUIRE(values.size() == 0_z);
        REQUIRE(values.capacity() == 0_z);
      }
    }

    WHEN("We add an element")
    {
      values.push_back(a);

      THEN("It is no longer empty and has size 1")
      {
        REQUIRE(!values.empty());
        REQUIRE(values.size() == 1_z);
        REQUIRE(values.capacity() >= 1_z);
        REQUIRE(values.at(0) == a);
        REQUIRE(values.front() == a);
        REQUIRE(values.back() == a);
      }
      AND_THEN("When we pop the element")
      {
        values.pop_back();

        THEN("It is empty again")
        {
          REQUIRE(values.empty());
          REQUIRE(values.size() == 0_z);
          REQUIRE(values.capacity() >= 0_z);
        }
      }
    }

    WHEN("We add multiple elements")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      THEN("It has the correct size and elements")
      {
        REQUIRE(!values.empty());
        REQUIRE(values.size() == 3_z);
        REQUIRE(values.capacity() >= 3_z);
        REQUIRE(values.at(0) == a);
        REQUIRE(values.at(1) == b);
        REQUIRE(values.at(2) == c);
        REQUIRE(values.front() == a);
        REQUIRE(values.back() == c);
      }
    }

    WHEN("We access an out-of-bounds index")
    {
      THEN("It throws an out_of_range exception")
      {
        REQUIRE_THROWS_AS(values.at(0), std::out_of_range);
      }
    }
  }
}