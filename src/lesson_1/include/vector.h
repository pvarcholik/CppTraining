#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <functional>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include <type_traits>

#include "default_growth_policy.h"
#include "literal_operators.h"

namespace CppTraining
{
template <typename T, typename Allocator = std::allocator<T>>
class Vector;

template <typename T, typename Allocator>
void swap(Vector<T, Allocator>& lhs, Vector<T, Allocator>& rhs) noexcept;

template <typename T, typename Allocator>
class Vector final
{
public:
  using size_type = std::size_t;
  using value_type = T;
  using reference = value_type&;
  using const_reference = const value_type&;
  using rvalue_reference = value_type&&;

  using allocator_type = Allocator;
  using allocator_traits = std::allocator_traits<Allocator>;

  using growth_policy_type = std::function<std::size_t(std::size_t, std::size_t)>;

  class Iterator final
  {
    friend Vector;

  public:
    using size_type = Vector::size_type;
    using value_type = Vector::value_type;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using pointer = value_type*;
    using iterator_category = std::random_access_iterator_tag;

    Iterator() = default;
    Iterator(const Iterator&) = default;
    Iterator(Iterator&&) noexcept = default;
    Iterator& operator=(const Iterator&) = default;
    Iterator& operator=(Iterator&&) noexcept = default;
    ~Iterator() = default;

    reference operator*() const;
    pointer operator->() const;

    bool operator==(Iterator rhs) const;
    bool operator!=(Iterator rhs) const;
    bool operator<(Iterator rhs) const;
    bool operator>(Iterator rhs) const;

    Iterator& operator++();
    Iterator operator++(int);
    Iterator& operator--();
    Iterator operator--(int);

    difference_type operator-(Iterator rhs) const;
    Iterator& operator+=(difference_type offset);
    Iterator& operator-=(difference_type offset);
    reference operator[](size_type index) const;

    friend Iterator operator+(const Iterator& position, difference_type offset)
    {
      if (position.container_ == nullptr)
      {
        throw std::runtime_error("Unassociated iterator.");
      }

      auto result = position;
      result += offset;
      return result;
    }

    friend Iterator operator+(difference_type offset, const Iterator& position)
    {
      return position + offset;
    }

    friend Iterator operator-(const Iterator& position, difference_type offset)
    {
      if (position.container_ == nullptr)
      {
        throw std::runtime_error("Unassociated iterator.");
      }

      auto result = position;
      result -= offset;
      return result;
    }

  private:
    Iterator(Vector& owner, size_type index);

    Vector* container_{nullptr};
    size_type index_{0_z};
  };

  class ConstIterator final
  {
    friend Vector;

  public:
    using size_type = Vector::size_type;
    using value_type = Vector::value_type;
    using difference_type = std::ptrdiff_t;
    using reference = const value_type&;
    using pointer = const value_type*;
    using iterator_category = std::random_access_iterator_tag;

    ConstIterator() = default;
    ConstIterator(const Iterator& other);
    ConstIterator(const ConstIterator&) = default;
    ConstIterator(ConstIterator&&) noexcept = default;
    ConstIterator& operator=(const ConstIterator&) = default;
    ConstIterator& operator=(ConstIterator&&) noexcept = default;
    ~ConstIterator() = default;

    reference operator*() const;
    pointer operator->() const;

    bool operator==(ConstIterator rhs) const;
    bool operator!=(ConstIterator rhs) const;
    bool operator<(ConstIterator rhs) const;
    bool operator>(ConstIterator rhs) const;

    ConstIterator& operator++();
    ConstIterator operator++(int);
    ConstIterator& operator--();
    ConstIterator operator--(int);

    difference_type operator-(ConstIterator rhs) const;
    ConstIterator& operator+=(difference_type offset);
    ConstIterator& operator-=(difference_type offset);
    reference operator[](size_type index) const;

    friend ConstIterator operator+(const ConstIterator& position, difference_type offset)
    {
      if (position.container_ == nullptr)
      {
        throw std::runtime_error("Unassociated iterator.");
      }

      auto result = position;
      result += offset;
      return result;
    }

    friend ConstIterator operator+(difference_type offset, const ConstIterator& position)
    {
      return position + offset;
    }

    friend ConstIterator operator-(const ConstIterator& position, difference_type offset)
    {
      if (position.container_ == nullptr)
      {
        throw std::runtime_error("Unassociated iterator.");
      }

      auto result = position;
      result -= offset;
      return result;
    }

  private:
    ConstIterator(const Vector& owner, size_type index);

    const Vector* container_{nullptr};
    size_type index_{0_z};
  };

  static_assert(std::is_copy_constructible<T>::value,
                "Vector<T> requires copy-constructible value type");

  explicit Vector(size_type capacity = 0_z,
                  const allocator_type& allocator = allocator_type{},
                  growth_policy_type growth_policy = DefaultGrowthPolicy{});
  Vector(std::initializer_list<value_type> values,
         const allocator_type& alloc = allocator_type(),
         growth_policy_type growth_policy = DefaultGrowthPolicy{});
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

  template <typename... Args>
  Iterator emplace_back(Args&&... args);

  Iterator push_back(const_reference value);
  Iterator push_back(rvalue_reference value);
  void pop_back();

  Iterator begin() noexcept;
  ConstIterator begin() const noexcept;
  ConstIterator cbegin() const noexcept;
  Iterator end() noexcept;
  ConstIterator end() const noexcept;
  ConstIterator cend() const noexcept;

private:
  size_type size_{0_z};
  size_type capacity_{0_z};
  value_type* data_{nullptr};
  allocator_type allocator_{};
  growth_policy_type growth_policy_;
};
} // namespace CppTraining

#include "vector.inl"
