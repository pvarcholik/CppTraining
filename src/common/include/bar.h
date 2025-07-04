#pragma once

#include <cstdint>
#include <string>

namespace CppTraining
{
/// @brief Bar is not copyable, but movable.
class Bar final
{
public:
  Bar() = default;
  Bar(const Bar&) = delete;
  Bar& operator=(const Bar&) = delete;
  Bar(Bar&&) noexcept = default;
  Bar& operator=(Bar&&) noexcept = default;
  ~Bar() = default;
};

} // namespace CppTraining
