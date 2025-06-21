#pragma once

#include <cstdint>
#include <algorithm>
#include <cassert>

#include "literal_operators.h"

namespace CppTraining
{
  template <typename T>
  class Vector final
  {
  public:
    using size_type = std::size_t;
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using rvalue_reference = value_type&&;

    explicit Vector(size_type capacity = 0_z);
    Vector(const Vector& other);
    Vector(Vector&& other) noexcept;
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    ~Vector();

    size_type size() const noexcept;
    size_type capacity() const noexcept;
    bool empty() const noexcept;

    void reserve(size_type capacity);
    void resize(size_type size);
    void clear();
    void shrink_to_fit();

  private:
    size_type size_{0_z};
    size_type capacity_{0_z};
    value_type* data_{nullptr};
  };

  template <typename T>
  void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept;
}

#include "vector.inl"