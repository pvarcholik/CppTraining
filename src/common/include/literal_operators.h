#pragma once

#include <cstddef>

inline std::size_t operator"" _z(unsigned long long int x)
{
  return static_cast<std::size_t>(x);
}
