#include <type_traits>

#include <catch2/catch_all.hpp>

#include "bar.h"
#include "foo.h"
#include "vector.h"

using namespace CppTraining;

TEST_CASE("Static interface checks", "[traits]")
{
  STATIC_REQUIRE(std::is_default_constructible<Foo>::value);
  STATIC_REQUIRE(std::is_copy_constructible<Foo>::value);
  STATIC_REQUIRE(std::is_move_assignable<Vector<Foo>>::value);
}

// Or

static_assert(std::is_copy_constructible<Foo>::value, "Foo must be copy-constructible");
static_assert(std::is_move_constructible<Foo>::value, "Foo must be move-constructible");
static_assert(std::is_default_constructible<Foo>::value, "Foo must be default-constructible");
static_assert(!std::is_trivially_copyable<Foo>::value, "Foo should not be trivially copyable");

static_assert(std::is_copy_constructible<Vector<Foo>>::value,
              "Vector<Foo> must be copy-constructible");
static_assert(std::is_move_constructible<Vector<Foo>>::value,
              "Vector<Foo> must be move-constructible");

// Or with helper macros
#define STATIC_REQUIRE_TRAIT(trait, type)                                                          \
  static_assert(trait<type>::value, #trait " failed for " #type)

#define STATIC_FORBID_TRAIT(trait, type)                                                           \
  static_assert(!trait<type>::value, #trait " unexpectedly passed for " #type)

// Traits checks for Foo
STATIC_REQUIRE_TRAIT(std::is_default_constructible, Foo);
STATIC_REQUIRE_TRAIT(std::is_copy_constructible, Foo);
STATIC_REQUIRE_TRAIT(std::is_move_assignable, Foo);
STATIC_FORBID_TRAIT(std::is_trivially_copyable, Foo);

// Traits checks for Vector<Foo>
STATIC_REQUIRE_TRAIT(std::is_copy_constructible, Vector<Foo>);
STATIC_REQUIRE_TRAIT(std::is_move_constructible, Vector<Foo>);

// Traits checks for Bar
STATIC_REQUIRE_TRAIT(std::is_default_constructible, Bar);
STATIC_FORBID_TRAIT(std::is_copy_constructible, Bar);
STATIC_REQUIRE_TRAIT(std::is_move_constructible, Bar);

// Vector<T> requires T to be copy-constructible
// Vector<Bar> vector_bar; // This will generate a static assert failure

SCENARIO("Exercising vector", "[vector]")
{
  GIVEN("An empty vector<Foo>")
  {
    const Foo a{1};
    const Foo b{2};
    const Foo c{3};
    const Foo d{4};

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

    WHEN("We reserve more than current capacity")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      auto old_capacity = values.capacity();
      values.reserve(old_capacity + 5_z);

      THEN("The capacity increases but size remains the same")
      {
        REQUIRE(values.size() == 3_z);
        REQUIRE(values.capacity() > old_capacity);
      }
    }

    WHEN("We reserve less than current capacity")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      auto old_capacity = values.capacity();
      values.reserve(2_z);

      THEN("The capacity remains unchanged")
      {
        REQUIRE(values.size() == 3_z);
        REQUIRE(values.capacity() == old_capacity);
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

    WHEN("We resize it to 0 elements")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      values.resize(0_z);

      THEN("It is empty again")
      {
        REQUIRE(values.empty());
        REQUIRE(values.size() == 0_z);
        REQUIRE(values.capacity() >= 0_z);
      }
    }

    WHEN("We resize it to a smaller size")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      values.resize(2_z);

      THEN("It has size 2 and retains the first two elements")
      {
        REQUIRE(!values.empty());
        REQUIRE(values.size() == 2_z);
        REQUIRE(values.capacity() >= 2_z);
        REQUIRE(values.at(0) == a);
        REQUIRE(values.at(1) == b);
        REQUIRE(values.front() == a);
        REQUIRE(values.back() == b);
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

    WHEN("We shrink it to fit with no elements")
    {
      values.shrink_to_fit();

      THEN("It is empty and capacity is zero")
      {
        REQUIRE(values.empty());
        REQUIRE(values.size() == 0_z);
        REQUIRE(values.capacity() == 0_z);
      }
    }

    WHEN("We shrink it to fit with 2 elements")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);
      values.pop_back();
      values.shrink_to_fit();

      THEN("It is size is 2 and capacity is >= 2")
      {
        REQUIRE(values.size() == 2_z);
        REQUIRE(values.capacity() >= 2_z);
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

    WHEN("We add an rvalue reference")
    {
      values.push_back(Foo{42});

      THEN("It has size 1 and the element is correctly added")
      {
        REQUIRE(!values.empty());
        REQUIRE(values.size() == 1_z);
        REQUIRE(values.capacity() >= 1_z);
        REQUIRE(values.at(0).getData() == 42);
      }
    }

    WHEN("We access the first element")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      THEN("It returns the first element")
      {
        REQUIRE(values.front() == a);
      }
    }

    WHEN("We access the last element")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      THEN("It returns the last element")
      {
        REQUIRE(values.back() == c);
      }
    }

    WHEN("We access an existing index")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      THEN("It returns the correct element")
      {
        REQUIRE(values.at(0) == a);
        REQUIRE(values.at(1) == b);
        REQUIRE(values.at(2) == c);
      }
    }

    WHEN("We access an out-of-bounds index")
    {
      THEN("It throws an out_of_range exception")
      {
        REQUIRE_THROWS_AS(values.at(0), std::out_of_range);
      }
    }

    WHEN("We copy the vector")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      Vector<Foo> other{values};

      THEN("The copy has the same size and elements")
      {
        REQUIRE(other.size() == values.size());
        REQUIRE(other.capacity() >= values.capacity());
        for (std::size_t i = 0; i < values.size(); ++i)
        {
          REQUIRE(other[i] == values[i]);
        }
      }
    }

    WHEN("We assign the vector to another")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      Vector<Foo> other;

      other = values;

      THEN("The other vector has the same size and elements")
      {
        REQUIRE(other.size() == values.size());
        REQUIRE(other.capacity() >= values.capacity());
        for (std::size_t i = 0; i < values.size(); ++i)
        {
          REQUIRE(other[i] == values[i]);
        }
      }
    }

    WHEN("We assign a vector to another with existing elements")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      Vector<Foo> other;
      other.push_back(d);

      other = values;

      THEN("The other vector has the same size and elements")
      {
        REQUIRE(other.size() == 3_z);
        REQUIRE(other.capacity() >= 3_z);
        REQUIRE(other.at(0) == a);
        REQUIRE(other.at(1) == b);
        REQUIRE(other.at(2) == c);
      }
    }

    WHEN("We assign a vector to itself")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      values = values;

      THEN("It remains unchanged")
      {
        REQUIRE(values.size() == 3_z);
        REQUIRE(values.capacity() >= 3_z);
        REQUIRE(values.at(0) == a);
        REQUIRE(values.at(1) == b);
        REQUIRE(values.at(2) == c);
      }
    }

    WHEN("We move the vector")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      Vector<Foo> other{std::move(values)};

      THEN("The moved vector has the same size and elements, and original is empty")
      {
        REQUIRE(other.size() == 3_z);
        REQUIRE(other.capacity() >= 3_z);
        REQUIRE(values.empty());
        REQUIRE(values.size() == 0_z);
        REQUIRE(values.capacity() >= 0_z);
      }
    }

    WHEN("We move assign the vector to another")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      Vector<Foo> other;
      other = std::move(values);

      THEN("The moved vector has the same size and elements, and original is empty")
      {
        REQUIRE(other.size() == 3_z);
        REQUIRE(other.capacity() >= 3_z);
        REQUIRE(values.empty());
        REQUIRE(values.size() == 0_z);
        REQUIRE(values.capacity() >= 0_z);
      }
    }

    WHEN("We move assign the vector to another with existing elements")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      Vector<Foo> other;
      other.push_back(d);

      other = std::move(values);

      THEN("The moved vector has the same size and elements, and original is empty")
      {
        REQUIRE(other.size() == 3_z);
        REQUIRE(other.capacity() >= 3_z);
        REQUIRE(values.empty());
        REQUIRE(values.size() == 0_z);
        REQUIRE(values.capacity() >= 0_z);
      }
    }

    WHEN("We swap two vectors")
    {
      Vector<Foo> other;
      other.push_back(a);
      other.push_back(b);

      swap(values, other);

      THEN("The vectors have swapped sizes and elements")
      {
        REQUIRE(values.size() == 2_z);
        REQUIRE(values.at(0) == a);
        REQUIRE(values.at(1) == b);
        REQUIRE(other.size() == 0_z);
        REQUIRE(other.empty());
      }
    }

    WHEN("We swap a vector with itself")
    {
      values.push_back(a);
      values.push_back(b);

      swap(values, values);

      THEN("The vector remains unchanged")
      {
        REQUIRE(values.size() == 2_z);
        REQUIRE(values.at(0) == a);
        REQUIRE(values.at(1) == b);
      }
    }

    WHEN("We access elements from a const view")
    {
      values.push_back(a);
      values.push_back(b);
      values.push_back(c);

      const Vector<Foo>& const_ref = values;

      THEN("We can retrieve size, capacity, and check emptiness")
      {
        REQUIRE(!const_ref.empty());
        REQUIRE(const_ref.size() == 3_z);
        REQUIRE(const_ref.capacity() >= 3_z);
      }

      THEN("We can access elements using const front/back/[]/at")
      {
        REQUIRE(const_ref[0_z] == a);
        REQUIRE(const_ref.at(1_z) == b);
        REQUIRE(const_ref.front() == a);
        REQUIRE(const_ref.back() == c);
      }

      THEN("Out-of-bounds access still throws through const interface")
      {
        REQUIRE_THROWS_AS(const_ref.at(100_z), std::out_of_range);
      }
    }

    WHEN("We construct the vector from an initializer list")
    {
      Vector<Foo> values{a, b, c};

      THEN("The size is correct and all elements are in order")
      {
        REQUIRE(values.size() == 3_z);
        REQUIRE(values.at(0) == a);
        REQUIRE(values.at(1) == b);
        REQUIRE(values.at(2) == c);
      }
    }
  }
}
