#include "vector.h"

namespace CppTraining
{
template <typename T>
inline Vector<T>::Iterator::Iterator(Vector& owner, size_type index)
    : container_(&owner), index_(index)
{
}

template <typename T>
typename Vector<T>::Iterator::reference Vector<T>::Iterator::operator*() const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return container_->at(index_);
}

template <typename T>
typename Vector<T>::Iterator::pointer Vector<T>::Iterator::operator->() const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return &(container_->at(index_));
}

template <typename T>
inline bool Vector<T>::Iterator::operator==(Iterator rhs) const
{
  return !(operator!=(rhs));
}

template <typename T>
inline bool Vector<T>::Iterator::operator!=(Iterator rhs) const
{
  return container_ != rhs.container_ || index_ != rhs.index_;
}

template <typename T>
bool Vector<T>::Iterator::operator<(Iterator rhs) const
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

template <typename T>
bool Vector<T>::Iterator::operator>(Iterator rhs) const
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

template <typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator++()
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  ++index_;

  return *this;
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::Iterator::operator++(int)
{
  Iterator temp{*this};
  operator++();
  return temp;
}

template <typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator--()
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

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::Iterator::operator--(int)
{
  Iterator temp = *this;
  operator--();
  return temp;
}

template <typename T>
typename Vector<T>::Iterator::difference_type Vector<T>::Iterator::operator-(Iterator rhs) const
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

template <typename T>
typename Vector<T>::Iterator& Vector<T>::Iterator::operator+=(difference_type offset)
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

template <typename T>
inline typename Vector<T>::Iterator& Vector<T>::Iterator::operator-=(difference_type offset)
{
  return *this += -offset;
}

template <typename T>
inline typename Vector<T>::Iterator::reference
Vector<T>::Iterator::operator[](size_type index) const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return container_->operator[](index);
}

template <typename T>
inline Vector<T>::ConstIterator::ConstIterator(const Iterator& other)
    : container_(other.container_), index_(other.index_)
{
}

template <typename T>
inline Vector<T>::ConstIterator::ConstIterator(const Vector& owner, size_type index)
    : container_(&owner), index_(index)
{
}

template <typename T>
typename Vector<T>::ConstIterator::reference Vector<T>::ConstIterator::operator*() const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return container_->at(index_);
}

template <typename T>
typename Vector<T>::ConstIterator::pointer Vector<T>::ConstIterator::operator->() const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return &(container_->at(index_));
}

template <typename T>
inline bool Vector<T>::ConstIterator::operator==(ConstIterator rhs) const
{
  return !(operator!=(rhs));
}

template <typename T>
inline bool Vector<T>::ConstIterator::operator!=(ConstIterator rhs) const
{
  return container_ != rhs.container_ || index_ != rhs.index_;
}

template <typename T>
bool Vector<T>::ConstIterator::operator<(ConstIterator rhs) const
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

template <typename T>
bool Vector<T>::ConstIterator::operator>(ConstIterator rhs) const
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

template <typename T>
typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator++()
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  ++index_;

  return *this;
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator++(int)
{
  ConstIterator temp{*this};
  operator++();
  return temp;
}

template <typename T>
typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator--()
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

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::ConstIterator::operator--(int)
{
  ConstIterator temp = *this;
  operator--();
  return temp;
}

template <typename T>
typename Vector<T>::ConstIterator::difference_type
Vector<T>::ConstIterator::operator-(ConstIterator rhs) const
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

template <typename T>
typename Vector<T>::ConstIterator& Vector<T>::ConstIterator::operator+=(difference_type offset)
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

template <typename T>
inline typename Vector<T>::ConstIterator&
Vector<T>::ConstIterator::operator-=(difference_type offset)
{
  return *this += -offset;
}

template <typename T>
inline typename Vector<T>::ConstIterator::reference
Vector<T>::ConstIterator::operator[](size_type index) const
{
  if (container_ == nullptr)
  {
    throw std::runtime_error("Unassociated iterator.");
  }

  return container_->operator[](index);
}

template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept
{
  using std::swap;
  swap(lhs.size_, rhs.size_);
  swap(lhs.capacity_, rhs.capacity_);
  swap(lhs.data_, rhs.data_);
}

template <typename T>
Vector<T>::Vector(size_type capacity, IncrementFunctor increment_functor)
    : increment_functor_{std::move(increment_functor)}
{
  reserve(capacity);
}

template <typename T>
Vector<T>::Vector(std::initializer_list<value_type> values) : Vector(values.size())
{
  for (const auto& value : values)
  {
    push_back(value);
  }
}

template <typename T>
Vector<T>::Vector(const Vector& other) : Vector{other.capacity_}
{
  if (capacity_ > 0_z)
  {
    for (size_type i = 0_z; i < other.size_; ++i)
    {
      new (data_ + i) value_type(other.data_[i]);
    }

    size_ = other.size_;
  }
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
    : size_{other.size_}, capacity_{other.capacity_}, data_{other.data_}
{
  other.size_ = 0_z;
  other.capacity_ = 0_z;
  other.data_ = nullptr;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
  auto copy{other};
  swap(*this, copy);

  return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
{
  if (this != &other)
  {
    clear();
    shrink_to_fit();

    size_ = other.size_;
    capacity_ = other.capacity_;
    data_ = other.data_;

    other.size_ = 0_z;
    other.capacity_ = 0_z;
    other.data_ = nullptr;
  }

  return *this;
}

template <typename T>
Vector<T>::~Vector() noexcept
{
  for (size_type i = 0_z; i < size_; ++i)
  {
    data_[i].~value_type();
  }

  ::operator delete(data_);
}

template <typename T>
typename Vector<T>::reference Vector<T>::operator[](size_type index)
{
  if (index >= size_)
  {
    throw std::out_of_range("Index out of range.");
  }

  return data_[index];
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::operator[](size_type index) const
{
  if (index >= size_)
  {
    throw std::out_of_range("Index out of range.");
  }

  return data_[index];
}

template <typename T>
inline typename Vector<T>::size_type Vector<T>::size() const noexcept
{
  return size_;
}

template <typename T>
inline typename Vector<T>::size_type Vector<T>::capacity() const noexcept
{
  return capacity_;
}

template <typename T>
inline bool Vector<T>::empty() const noexcept
{
  return size_ == 0_z;
}

template <typename T>
inline typename Vector<T>::reference Vector<T>::front()
{
  return operator[](0_z);
}

template <typename T>
inline typename Vector<T>::const_reference Vector<T>::front() const
{
  return operator[](0_z);
}

template <typename T>
inline typename Vector<T>::reference Vector<T>::back()
{
  return operator[](size_ - 1_z);
}

template <typename T>
inline typename Vector<T>::const_reference Vector<T>::back() const
{
  return operator[](size_ - 1_z);
}

template <typename T>
inline typename Vector<T>::reference Vector<T>::at(size_type index)
{
  return operator[](index);
}

template <typename T>
inline typename Vector<T>::const_reference Vector<T>::at(size_type index) const
{
  return operator[](index);
}

template <typename T>
void Vector<T>::reserve(size_type new_capacity)
{
  if (new_capacity > capacity_)
  {
    auto new_data{reinterpret_cast<value_type*>(::operator new(new_capacity * sizeof(value_type)))};

    for (size_type i = 0_z; i < size_; ++i)
    {
      new (new_data + i) value_type(std::move(data_[i]));
      data_[i].~value_type();
    }

    ::operator delete(data_);
    data_ = new_data;
    capacity_ = new_capacity;
  }
}

template <typename T>
void Vector<T>::resize(size_type new_size)
{
  if (new_size < size_)
  {
    for (size_type i = new_size; i < size_; ++i)
    {
      data_[i].~value_type();
    }

    if (new_size == 0_z)
    {
      ::operator delete(data_);
      data_ = nullptr;
      capacity_ = size_ = 0_z;
    }
    else
    {
      auto new_data{reinterpret_cast<value_type*>(::operator new(new_size * sizeof(value_type)))};

      for (size_type i = 0_z; i < new_size; ++i)
      {
        new (new_data + i) value_type(std::move(data_[i]));
        data_[i].~value_type();
      }

      ::operator delete(data_);
      data_ = new_data;
      capacity_ = size_ = new_size;
    }
  }
  else if (new_size > size_)
  {
    reserve(new_size);

    for (size_type i = size_; i < new_size; ++i)
    {
      new (data_ + i) value_type{};
    }

    size_ = new_size;
  }

  assert(capacity_ == size_);
}

template <typename T>
void Vector<T>::clear()
{
  for (size_type i = 0_z; i < size_; ++i)
  {
    data_[i].~value_type();
  }

  size_ = 0_z;
}

template <typename T>
void Vector<T>::shrink_to_fit()
{
  if (size_ == 0)
  {
    ::operator delete(data_);
    data_ = nullptr;
    capacity_ = 0;
  }
  else if (size_ != capacity_)
  {
    auto new_data{reinterpret_cast<value_type*>(::operator new(size_ * sizeof(value_type)))};

    for (size_type i = 0_z; i < size_; ++i)
    {
      new (new_data + i) value_type(std::move(data_[i]));
      data_[i].~value_type();
    }

    ::operator delete(data_);
    data_ = new_data;
    capacity_ = size_;
  }
}

template <typename T>
template <typename... Args>
typename Vector<T>::Iterator Vector<T>::emplace_back(Args&&... args)
{
  if (size_ == capacity_)
  {
    std::size_t capacity = capacity_ + std::max(1_z, increment_functor_(size_, capacity_));
    reserve(capacity);
  }

  new (data_ + size_) value_type(std::forward<Args>(args)...);
  return {*this, size_++};
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::push_back(const_reference value)
{
  return emplace_back(value);
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::push_back(rvalue_reference value)
{
  return emplace_back(std::move(value));
}

template <typename T>
void Vector<T>::pop_back()
{
  if (size_ > 0)
  {
    data_[--size_].~value_type();
  }
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::begin() noexcept
{
  return Iterator(*this, 0);
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::begin() const noexcept
{
  return ConstIterator(*this, 0);
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::cbegin() const noexcept
{
  return ConstIterator(*this, 0);
}

template <typename T>
inline typename Vector<T>::Iterator Vector<T>::end() noexcept
{
  return Iterator(*this, size_);
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::end() const noexcept
{
  return ConstIterator(*this, size_);
}

template <typename T>
inline typename Vector<T>::ConstIterator Vector<T>::cend() const noexcept
{
  return ConstIterator(*this, size_);
}
} // namespace CppTraining
