#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <stdexcept>
#include <type_traits>

#include "literal_operators.h"

namespace CppTraining
{
template <typename T>
class Vector;

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept;

template <typename T>
class Vector final
{
public:
  using size_type = std::size_t;
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using rvalue_reference = value_type&&;

  static_assert(std::is_copy_constructible<T>::value,
                "Vector<T> requires copy-constructible value type");

  explicit Vector(size_type capacity = 0_z);
  Vector(std::initializer_list<value_type> values);
  Vector(const Vector& other);
  Vector(Vector&& other) noexcept;
  Vector& operator=(const Vector& other);
  Vector& operator=(Vector&& other) noexcept;
  ~Vector() noexcept;

  friend void swap<>(Vector& lhs, Vector& rhs) noexcept;

  reference operator[](size_type index);
  const_reference operator[](size_type index) const;

  size_type size() const noexcept;
  size_type capacity() const noexcept;
  bool empty() const noexcept;

  reference front();
  const_reference front() const;
  reference back();
  const_reference back() const;
  reference at(size_type index);
  const_reference at(size_type index) const;

  void reserve(size_type capacity);
  void resize(size_type size);
  void clear();
  void shrink_to_fit();

  void push_back(const_reference value);
  void push_back(rvalue_reference value);
  void pop_back();

private:
  size_type size_{0_z};
  size_type capacity_{0_z};
  value_type* data_{nullptr};
};
} // namespace CppTraining

#include "vector.inl"
