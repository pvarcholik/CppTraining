#pragma once

#include <cstddef>

namespace CppTraining
{
struct DefaultGrowthPolicy final
{
  std::size_t operator()(std::size_t /* size */, std::size_t capacity) const { return capacity; }
};
} // namespace CppTraining
