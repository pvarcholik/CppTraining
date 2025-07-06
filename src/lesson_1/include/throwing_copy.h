#pragma once

#include <cstddef>
#include <cstdint>
#include <limits>
#include <stdexcept>

#include "literal_operators.h"

namespace CppTraining
{
class ThrowingCopy final
{
public:
  using size_type = std::size_t;

  explicit ThrowingCopy(std::int32_t value) : value_{value} {}

  // Copy constructor throws based on copy counter
  ThrowingCopy(const ThrowingCopy& other) : value_(other.value_)
  {
    if (++copy_count_ == throw_on_)
    {
      throw std::runtime_error("Simulated copy failure");
    }
  }

  ThrowingCopy(ThrowingCopy&& other) noexcept = default;
  ThrowingCopy& operator=(const ThrowingCopy&) = default;
  ThrowingCopy& operator=(ThrowingCopy&&) noexcept = default;
  ~ThrowingCopy() = default;

  bool operator==(const ThrowingCopy& rhs) const { return value_ == rhs.value_; }
  bool operator!=(const ThrowingCopy& rhs) const { return !(*this == rhs); }

  std::int32_t getData() const { return value_; }

  static void reset(size_type throw_on)
  {
    copy_count_ = 0;
    throw_on_ = throw_on;
  }

private:
  std::int32_t value_{0};
  static size_type copy_count_;
  static size_type throw_on_;
};

inline ThrowingCopy::size_type ThrowingCopy::copy_count_{0};
inline ThrowingCopy::size_type ThrowingCopy::throw_on_{
  std::numeric_limits<ThrowingCopy::size_type>::max()};

} // namespace CppTraining
