#pragma once

#include <cstddef>

[[nodiscard]] inline std::size_t operator"" _z(unsigned long long int x)
{
  return static_cast<size_t>(x);
}
