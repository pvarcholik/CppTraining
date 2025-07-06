#include "vector.h"

namespace CppTraining
{
template <typename T, typename Allocator>
inline Vector<T, Allocator>::Iterator::Iterator(Vector& owner, size_type index)
    : container_(&owner), index_(index)
{
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator::reference Vector<T, Allocator>::Iterator::operator*() const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return container_->at(index_);
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator::pointer Vector<T, Allocator>::Iterator::operator->() const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return &(container_->at(index_));
}

template <typename T, typename Allocator>
inline bool Vector<T, Allocator>::Iterator::operator==(Iterator rhs) const
{
  return !(operator!=(rhs));
}

template <typename T, typename Allocator>
inline bool Vector<T, Allocator>::Iterator::operator!=(Iterator rhs) const
{
  return container_ != rhs.container_ || index_ != rhs.index_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::Iterator::operator<(Iterator rhs) const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  if (rhs.container_ != container_)
  {
    throw std::runtime_error("Unrelated iterators.");
  }

  return index_ < rhs.index_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::Iterator::operator>(Iterator rhs) const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  if (rhs.container_ != container_)
  {
    throw std::runtime_error("Unrelated iterators.");
  }

  return index_ > rhs.index_;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator++()
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  ++index_;

  return *this;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::Iterator::operator++(int)
{
  Iterator temp{*this};
  operator++();
  return temp;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator& Vector<T, Allocator>::Iterator::operator--()
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  if (index_ > 0)
  {
    --index_;
  }

  return *this;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::Iterator::operator--(int)
{
  Iterator temp = *this;
  operator--();
  return temp;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator::difference_type
Vector<T, Allocator>::Iterator::operator-(Iterator rhs) const
{
  if (container_ != rhs.container_)
  {
    throw std::runtime_error("Cannot subtract iterators of different containers.");
  }

  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return index_ - rhs.index_;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator&
Vector<T, Allocator>::Iterator::operator+=(difference_type offset)
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  if (offset < 0)
  {
    size_type decrement = static_cast<size_type>(-offset);
    index_ = decrement > index_ ? 0 : index_ - decrement;
  }
  else
  {
    index_ += static_cast<size_type>(offset);
  }

  return *this;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::Iterator&
Vector<T, Allocator>::Iterator::operator-=(difference_type offset)
{
  return *this += -offset;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::Iterator::reference
Vector<T, Allocator>::Iterator::operator[](size_type index) const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return container_->operator[](index);
}

template <typename T, typename Allocator>
inline Vector<T, Allocator>::ConstIterator::ConstIterator(const Iterator& other)
    : container_(other.container_), index_(other.index_)
{
}

template <typename T, typename Allocator>
inline Vector<T, Allocator>::ConstIterator::ConstIterator(const Vector& owner, size_type index)
    : container_(&owner), index_(index)
{
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::ConstIterator::reference
Vector<T, Allocator>::ConstIterator::operator*() const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return container_->at(index_);
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::ConstIterator::pointer
Vector<T, Allocator>::ConstIterator::operator->() const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return &(container_->at(index_));
}

template <typename T, typename Allocator>
inline bool Vector<T, Allocator>::ConstIterator::operator==(ConstIterator rhs) const
{
  return !(operator!=(rhs));
}

template <typename T, typename Allocator>
inline bool Vector<T, Allocator>::ConstIterator::operator!=(ConstIterator rhs) const
{
  return container_ != rhs.container_ || index_ != rhs.index_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::ConstIterator::operator<(ConstIterator rhs) const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  if (rhs.container_ != container_)
  {
    throw std::runtime_error("Unrelated iterators.");
  }

  return index_ < rhs.index_;
}

template <typename T, typename Allocator>
bool Vector<T, Allocator>::ConstIterator::operator>(ConstIterator rhs) const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  if (rhs.container_ != container_)
  {
    throw std::runtime_error("Unrelated iterators.");
  }

  return index_ > rhs.index_;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::ConstIterator& Vector<T, Allocator>::ConstIterator::operator++()
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  ++index_;

  return *this;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::ConstIterator
Vector<T, Allocator>::ConstIterator::operator++(int)
{
  ConstIterator temp{*this};
  operator++();
  return temp;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::ConstIterator& Vector<T, Allocator>::ConstIterator::operator--()
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  if (index_ > 0)
  {
    --index_;
  }

  return *this;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::ConstIterator
Vector<T, Allocator>::ConstIterator::operator--(int)
{
  ConstIterator temp = *this;
  operator--();
  return temp;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::ConstIterator::difference_type
Vector<T, Allocator>::ConstIterator::operator-(ConstIterator rhs) const
{
  if (container_ != rhs.container_)
  {
    throw std::runtime_error("Cannot subtract iterators of different containers.");
  }

  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return index_ - rhs.index_;
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::ConstIterator&
Vector<T, Allocator>::ConstIterator::operator+=(difference_type offset)
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  if (offset < 0)
  {
    size_type decrement = static_cast<size_type>(-offset);
    index_ = decrement > index_ ? 0 : index_ - decrement;
  }
  else
  {
    index_ += static_cast<size_type>(offset);
  }

  return *this;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::ConstIterator&
Vector<T, Allocator>::ConstIterator::operator-=(difference_type offset)
{
  return *this += -offset;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::ConstIterator::reference
Vector<T, Allocator>::ConstIterator::operator[](size_type index) const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return container_->operator[](index);
}

template <typename T, typename Allocator>
void swap(Vector<T, Allocator>& lhs, Vector<T, Allocator>& rhs) noexcept
{
  using std::swap;
  using allocator_traits = std::allocator_traits<Allocator>;

  swap(lhs.size_, rhs.size_);
  swap(lhs.capacity_, rhs.capacity_);
  swap(lhs.data_, rhs.data_);

  if (allocator_traits::propagate_on_container_swap::value)
  {
    swap(lhs.allocator_, rhs.allocator_);
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(size_type capacity,
                             const allocator_type& allocator,
                             growth_policy_type growth_policy)
    : allocator_{allocator}, growth_policy_{std::move(growth_policy)}
{
  reserve(capacity);
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(std::initializer_list<value_type> values,
                             const allocator_type& allocator,
                             growth_policy_type growth_policy)
    : Vector(values.size(), allocator, growth_policy)
{
  for (const auto& value : values)
  {
    emplace_back(value);
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(const Vector& other)
    : capacity_{other.capacity_}
    , allocator_{allocator_traits::select_on_container_copy_construction(other.allocator_)}
{
  if (capacity_ > 0_z)
  {
    data_ = allocator_traits::allocate(allocator_, capacity_);

    size_type i = 0_z;
    try
    {
      for (; i < other.size_; ++i)
      {
        allocator_traits::construct(allocator_, data_ + i, other.data_[i]);
      }

      size_ = other.size_;
    }
    catch (...)
    {
      for (size_type j = 0_z; j < i; ++j)
      {
        allocator_traits::destroy(allocator_, data_ + i);
      }

      allocator_traits::deallocate(allocator_, data_, capacity_);

      throw;
    }
  }
}

template <typename T, typename Allocator>
Vector<T, Allocator>::Vector(Vector&& other) noexcept
    : size_{other.size_}
    , capacity_{other.capacity_}
    , data_{other.data_}
    , allocator_{std::move(other.allocator_)}
{
  other.size_ = 0_z;
  other.capacity_ = 0_z;
  other.data_ = nullptr;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(const Vector& other)
{
  if (this != &other)
  {
    if (allocator_traits::propagate_on_container_copy_assignment::value)
    {
      if (allocator_ != other.allocator_)
      {
        clear();
        shrink_to_fit();
      }
      allocator_ = other.allocator_;
    }

    // Manual copy assignment
    clear();
    reserve(other.capacity_);
    for (size_type i = 0_z; i < other.size_; ++i)
    {
      allocator_traits::construct(allocator_, data_ + i, other.data_[i]);
    }

    size_ = other.size_;
  }

  return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>& Vector<T, Allocator>::operator=(Vector&& other) noexcept
{
  if (this != &other)
  {
    if (allocator_traits::propagate_on_container_move_assignment::value)
    {
      // Safe to steal storage and allocator
      clear();
      shrink_to_fit();

      allocator_ = std::move(other.allocator_);
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
    }
    else
    {
      // Allocator is not propagated and we must use our own allocator
      if (allocator_ != other.allocator_)
      {
        clear();
        shrink_to_fit();

        reserve(other.size_);
        for (size_type i = 0_z; i < other.size_; ++i)
        {
          allocator_traits::construct(allocator_, data_ + i, std::move(other.data_[i]));
        }

        size_ = other.size_;
      }
      else
      {
        // Allocators match, safe to steal anyway
        clear();
        shrink_to_fit();

        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
      }
    }

    other.data_ = nullptr;
    other.size_ = 0_z;
    other.capacity_ = 0_z;
  }

  return *this;
}

template <typename T, typename Allocator>
Vector<T, Allocator>::~Vector() noexcept
{
  for (size_type i = 0_z; i < size_; ++i)
  {
    allocator_traits::destroy(allocator_, data_ + i);
  }

  if (data_ != nullptr)
  {
    allocator_traits::deallocate(allocator_, data_, capacity_);
  }
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::reference Vector<T, Allocator>::operator[](size_type index)
{
  if (index >= size_)
  {
    throw std::out_of_range("Index out of range.");
  }

  return data_[index];
}

template <typename T, typename Allocator>
typename Vector<T, Allocator>::const_reference
Vector<T, Allocator>::operator[](size_type index) const
{
  if (index >= size_)
  {
    throw std::out_of_range("Index out of range.");
  }

  return data_[index];
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::size_type Vector<T, Allocator>::size() const noexcept
{
  return size_;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::size_type Vector<T, Allocator>::capacity() const noexcept
{
  return capacity_;
}

template <typename T, typename Allocator>
inline bool Vector<T, Allocator>::empty() const noexcept
{
  return size_ == 0_z;
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::reference Vector<T, Allocator>::front()
{
  return operator[](0_z);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::front() const
{
  return operator[](0_z);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::reference Vector<T, Allocator>::back()
{
  return operator[](size_ - 1_z);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::const_reference Vector<T, Allocator>::back() const
{
  return operator[](size_ - 1_z);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::reference Vector<T, Allocator>::at(size_type index)
{
  return operator[](index);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::const_reference
Vector<T, Allocator>::at(size_type index) const
{
  return operator[](index);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::reserve(size_type new_capacity)
{
  if (new_capacity > capacity_)
  {
    // Allocate raw memory with the allocator
    auto new_data = allocator_traits::allocate(allocator_, new_capacity);

    for (size_type i = 0_z; i < size_; ++i)
    {
      allocator_traits::construct(allocator_, new_data + i, std::move(data_[i]));
      allocator_traits::destroy(allocator_, data_ + i);
    }

    if (data_)
    {
      allocator_traits::deallocate(allocator_, data_, capacity_);
    }

    data_ = new_data;
    capacity_ = new_capacity;
  }
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::resize(size_type new_size)
{
  if (new_size < size_)
  {
    for (size_type i = new_size; i < size_; ++i)
    {
      allocator_traits::destroy(allocator_, data_ + i);
    }

    size_ = new_size;

    if (new_size == 0)
    {
      allocator_traits::deallocate(allocator_, data_, capacity_);
      data_ = nullptr;
      capacity_ = 0;
    }
  }
  else if (new_size > size_)
  {
    reserve(new_size);

    for (size_type i = size_; i < new_size; ++i)
    {
      allocator_traits::construct(allocator_, data_ + i);
    }

    size_ = new_size;
  }

  assert(capacity_ >= size_);
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::clear()
{
  for (size_type i = 0_z; i < size_; ++i)
  {
    data_[i].~value_type();
  }

  size_ = 0_z;
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::shrink_to_fit()
{
  if (size_ == 0)
  {
    if (data_)
    {
      allocator_traits::deallocate(allocator_, data_, capacity_);
    }
    data_ = nullptr;
    capacity_ = 0_z;
  }
  else if (size_ != capacity_)
  {
    auto new_data = allocator_traits::allocate(allocator_, size_);

    for (size_type i = 0; i < size_; ++i)
    {
      allocator_traits::construct(allocator_, new_data + i, std::move(data_[i]));
      allocator_traits::destroy(allocator_, data_ + i);
    }

    allocator_traits::deallocate(allocator_, data_, capacity_);
    data_ = new_data;
    capacity_ = size_;
  }
}

template <typename T, typename Allocator>
template <typename... Args>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::emplace_back(Args&&... args)
{
  if (size_ == capacity_)
  {
    std::size_t capacity = capacity_ + std::max(1_z, growth_policy_(size_, capacity_));
    reserve(capacity);
  }

  allocator_traits::construct(allocator_, data_ + size_, std::forward<Args>(args)...);
  return {*this, size_++};
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::Iterator
Vector<T, Allocator>::push_back(const_reference value)
{
  return emplace_back(value);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::Iterator
Vector<T, Allocator>::push_back(rvalue_reference value)
{
  return emplace_back(std::move(value));
}

template <typename T, typename Allocator>
void Vector<T, Allocator>::pop_back()
{
  if (size_ > 0)
  {
    data_[--size_].~value_type();
  }
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::begin() noexcept
{
  return Iterator(*this, 0);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::ConstIterator Vector<T, Allocator>::begin() const noexcept
{
  return ConstIterator(*this, 0);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::ConstIterator Vector<T, Allocator>::cbegin() const noexcept
{
  return ConstIterator(*this, 0);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::end() noexcept
{
  return Iterator(*this, size_);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::ConstIterator Vector<T, Allocator>::end() const noexcept
{
  return ConstIterator(*this, size_);
}

template <typename T, typename Allocator>
inline typename Vector<T, Allocator>::ConstIterator Vector<T, Allocator>::cend() const noexcept
{
  return ConstIterator(*this, size_);
}
} // namespace CppTraining
