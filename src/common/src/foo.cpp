#include "foo.h"

#include <cassert>

namespace CppTraining
{
Foo::Foo(std::int32_t data) : data_{new std::int32_t(data)} {}

Foo::Foo(const Foo& rhs) : data_{new std::int32_t(*rhs.data_)} {}

Foo::Foo(Foo&& rhs) noexcept : data_{rhs.data_}
{
  rhs.data_ = nullptr;
}

Foo& Foo::operator=(const Foo& rhs)
{
  if (this != &rhs)
  {
    *data_ = *rhs.data_;
  }

  return *this;
}

Foo& Foo::operator=(Foo&& rhs) noexcept
{
  if (this != &rhs)
  {
    delete data_;
    data_ = rhs.data_;
    rhs.data_ = nullptr;
  }

  return *this;
}

Foo::~Foo()
{
  delete data_;
}

bool Foo::operator==(const Foo& rhs) const
{
  return *data_ == *rhs.data_;
}

bool Foo::operator!=(const Foo& rhs) const
{
  return !operator==(rhs);
}

std::int32_t Foo::getData() const
{
  assert(data_ != nullptr); // Guard against accessing a moved object
  return *data_;
}

void Foo::setData(std::int32_t data)
{
  assert(data_ != nullptr); // Guard against accessing a moved object
  *data_ = data;
}

std::string to_string(const Foo& value)
{
  return std::to_string(value.getData());
}
} // namespace CppTraining
