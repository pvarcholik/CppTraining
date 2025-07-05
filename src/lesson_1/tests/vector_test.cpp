#include <algorithm>

#include <catch2/catch_all.hpp>

#include "foo.h"
#include "vector.h"

using namespace CppTraining;

SCENARIO("Exercising Vector<T>", "[vector]")
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

template <typename Iterator, typename VectorType>
void testCommonIteratorOperations(VectorType& values, Iterator begin, Iterator end)
{
  const auto a = values.at(0);
  const auto b = values.at(1);
  const auto c = values.at(2);

  SECTION("Dereference and arrow operators")
  {
    REQUIRE(*begin == a);
    REQUIRE(begin->getData() == 1);
  }

  SECTION("Increment / Decrement (prefix and postfix)")
  {
    auto it = begin;
    REQUIRE(*it == a);

    ++it;
    REQUIRE(*it == b);

    it++;
    REQUIRE(*it == c);

    --it;
    REQUIRE(*it == b);

    it--;
    REQUIRE(*it == a);
  }

  SECTION("Addition (non-mutating)")
  {
    auto it1 = begin + 1;
    REQUIRE(*it1 == b);

    auto it2 = 1 + begin;
    REQUIRE(*it2 == b);
  }

  SECTION("Subtraction (non-mutating)")
  {
    auto it1 = end - 1;
    REQUIRE(*it1 == c);
  }

  SECTION("Compound assignment += / -=")
  {
    auto it = begin;
    it += 2;
    REQUIRE(*it == c);

    it -= 1;
    REQUIRE(*it == b);
  }

  SECTION("Distance between iterators")
  {
    auto mid = begin + 1;
    REQUIRE(end - begin == 3);
    REQUIRE(mid - begin == 1);
    REQUIRE(begin - mid == -1);
  }

  SECTION("Indexing via []")
  {
    REQUIRE(begin[0] == a);
    REQUIRE(begin[1] == b);
    REQUIRE(begin[2] == c);
  }

  SECTION("Comparison operators")
  {
    auto mid = begin + 1;
    REQUIRE(begin == begin);
    REQUIRE(mid != end);
    REQUIRE(mid < end);
    REQUIRE(end > mid);
    REQUIRE_FALSE(mid > end);
    REQUIRE_FALSE(end < mid);
  }
}

SCENARIO("Exercising Vector<T>::Iterator and Vector<T>::ConstIterator", "[vector_iterator]")
{
  using Iterator = Vector<Foo>::Iterator;
  using ConstIterator = Vector<Foo>::ConstIterator;

  GIVEN("A vector with three elements")
  {
    const Foo a{1};
    const Foo b{2};
    const Foo c{3};

    Vector<Foo> values{a, b, c};

    GIVEN("A non-const Vector")
    {
      testCommonIteratorOperations(values, values.begin(), values.end());
    }

    GIVEN("A const Vector")
    {
      const Vector<Foo>& const_values = values;
      testCommonIteratorOperations(const_values, const_values.begin(), const_values.end());
    }
  }

  GIVEN("A default-constructed iterator")
  {
    Iterator it{};

    SECTION("Dereferencing throws")
    {
      REQUIRE_THROWS_AS(*it, std::runtime_error);
    }

    SECTION("Arrow access throws")
    {
      REQUIRE_THROWS_AS(it->getData(), std::runtime_error);
    }

    SECTION("Incrementing (prefix) throws")
    {
      REQUIRE_THROWS_AS(++it, std::runtime_error);
    }

    SECTION("Incrementing (postfix) throws")
    {
      REQUIRE_THROWS_AS(it++, std::runtime_error);
    }

    SECTION("Decrementing (prefix) throws")
    {
      REQUIRE_THROWS_AS(--it, std::runtime_error);
    }

    SECTION("Decrementing (postfix) throws")
    {
      REQUIRE_THROWS_AS(it--, std::runtime_error);
    }

    SECTION("Addition throws")
    {
      REQUIRE_THROWS_AS(it + 1, std::runtime_error);
    }

    SECTION("Subtraction throws")
    {
      REQUIRE_THROWS_AS(it - 1, std::runtime_error);
    }

    SECTION("Compound += throws")
    {
      REQUIRE_THROWS_AS(it += 1, std::runtime_error);
    }

    SECTION("Compound -= throws")
    {
      REQUIRE_THROWS_AS(it -= 1, std::runtime_error);
    }

    SECTION("Indexing throws")
    {
      REQUIRE_THROWS_AS(it[0], std::runtime_error);
    }
  }

  GIVEN("Iterators from unrelated containers")
  {
    Vector<Foo> one;
    Vector<Foo> two;

    auto it1 = one.begin();
    auto it2 = two.begin();

    SECTION("Subtracting unrelated iterators throws")
    {
      REQUIRE_THROWS_AS(it1 - it2, std::runtime_error);
    }
  }

  GIVEN("An invalid iterator and a valid iterator")
  {
    Iterator invalid{};
    Vector<Foo> values{Foo{1}};
    auto valid = values.begin();

    SECTION("operator< throws when lhs is invalid")
    {
      REQUIRE_THROWS_AS(invalid < valid, std::runtime_error);
    }

    SECTION("operator< throws when rhs is invalid")
    {
      REQUIRE_THROWS_AS(valid < invalid, std::runtime_error);
    }

    SECTION("operator< throws when both are invalid")
    {
      REQUIRE_THROWS_AS(invalid < invalid, std::runtime_error);
    }

    SECTION("operator> throws when lhs is invalid")
    {
      REQUIRE_THROWS_AS(invalid > valid, std::runtime_error);
    }

    SECTION("operator> throws when rhs is invalid")
    {
      REQUIRE_THROWS_AS(valid > invalid, std::runtime_error);
    }

    SECTION("operator> throws when both are invalid")
    {
      REQUIRE_THROWS_AS(invalid > invalid, std::runtime_error);
    }

    SECTION("operator- throws when lhs is invalid")
    {
      REQUIRE_THROWS_AS(invalid - valid, std::runtime_error);
    }

    SECTION("operator- throws when rhs is invalid")
    {
      REQUIRE_THROWS_AS(valid - invalid, std::runtime_error);
    }

    SECTION("operator- throws when both are invalid")
    {
      REQUIRE_THROWS_AS(invalid - invalid, std::runtime_error);
    }
  }

  GIVEN("A valid iterator at begin()")
  {
    const Foo a{1};
    const Foo b{2};
    const Foo c{3};

    Vector<Foo> values{a, b, c};
    auto it = values.end();

    SECTION("Subtracting past begin() clamps at begin()")
    {
      auto result = values.end();
      result -= 4;
      REQUIRE(result == values.begin()); // clamp to begin()
    }
  }

  SECTION("std::sort works with iterators (if T is sortable)")
  {
    Vector<int> values{3, 1, 2};
    std::sort(values.begin(), values.end());
    REQUIRE(values.at(0) == 1);
    REQUIRE(values.at(1) == 2);
    REQUIRE(values.at(2) == 3);
  }

  GIVEN("A default-constructed ConstIterator")
  {
    ConstIterator it{};

    SECTION("Dereferencing throws")
    {
      REQUIRE_THROWS_AS(*it, std::runtime_error);
    }

    SECTION("Arrow access throws")
    {
      REQUIRE_THROWS_AS(it->getData(), std::runtime_error);
    }

    SECTION("Incrementing (prefix) throws")
    {
      REQUIRE_THROWS_AS(++it, std::runtime_error);
    }

    SECTION("Incrementing (postfix) throws")
    {
      REQUIRE_THROWS_AS(it++, std::runtime_error);
    }

    SECTION("Decrementing (prefix) throws")
    {
      REQUIRE_THROWS_AS(--it, std::runtime_error);
    }

    SECTION("Decrementing (postfix) throws")
    {
      REQUIRE_THROWS_AS(it--, std::runtime_error);
    }

    SECTION("Addition throws")
    {
      REQUIRE_THROWS_AS(it + 1, std::runtime_error);
    }

    SECTION("Subtraction throws")
    {
      REQUIRE_THROWS_AS(it - 1, std::runtime_error);
    }

    SECTION("Compound += throws")
    {
      REQUIRE_THROWS_AS(it += 1, std::runtime_error);
    }

    SECTION("Compound -= throws")
    {
      REQUIRE_THROWS_AS(it -= 1, std::runtime_error);
    }

    SECTION("Indexing throws")
    {
      REQUIRE_THROWS_AS(it[0], std::runtime_error);
    }
  }

  GIVEN("ConstIterators from unrelated containers")
  {
    const Vector<Foo> one;
    const Vector<Foo> two;

    auto it1 = one.begin();
    auto it2 = two.begin();

    SECTION("Subtracting unrelated ConstIterators throws")
    {
      REQUIRE_THROWS_AS(it1 - it2, std::runtime_error);
    }
  }

  GIVEN("An invalid ConstIterator and a valid ConstIterator")
  {
    ConstIterator invalid{};
    const Vector<Foo> values{Foo{1}};
    auto valid = values.begin();

    SECTION("operator< throws when lhs is invalid")
    {
      REQUIRE_THROWS_AS(invalid < valid, std::runtime_error);
    }

    SECTION("operator< throws when rhs is invalid")
    {
      REQUIRE_THROWS_AS(valid < invalid, std::runtime_error);
    }

    SECTION("operator< throws when both are invalid")
    {
      REQUIRE_THROWS_AS(invalid < invalid, std::runtime_error);
    }

    SECTION("operator> throws when lhs is invalid")
    {
      REQUIRE_THROWS_AS(invalid > valid, std::runtime_error);
    }

    SECTION("operator> throws when rhs is invalid")
    {
      REQUIRE_THROWS_AS(valid > invalid, std::runtime_error);
    }

    SECTION("operator> throws when both are invalid")
    {
      REQUIRE_THROWS_AS(invalid > invalid, std::runtime_error);
    }

    SECTION("operator- throws when lhs is invalid")
    {
      REQUIRE_THROWS_AS(invalid - valid, std::runtime_error);
    }

    SECTION("operator- throws when rhs is invalid")
    {
      REQUIRE_THROWS_AS(valid - invalid, std::runtime_error);
    }

    SECTION("operator- throws when both are invalid")
    {
      REQUIRE_THROWS_AS(invalid - invalid, std::runtime_error);
    }
  }

  GIVEN("A valid ConstIterator at cbegin()")
  {
    const Foo a{1};
    const Foo b{2};
    const Foo c{3};

    Vector<Foo> values{a, b, c};

    SECTION("Subtracting past cbegin() clamps at cbegin()")
    {
      auto result = values.cend();
      result -= 4;
      REQUIRE(result == values.cbegin()); // clamp to cbegin()
    }

    SECTION("ConstIterator can be constructed from Iterator")
    {
      Vector<Foo>::Iterator it = values.begin();
      ConstIterator const_it(it);
      REQUIRE(const_it == values.cbegin());
      REQUIRE(*const_it == *it);
    }
  }
}
