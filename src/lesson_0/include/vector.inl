#include "vector.h"

namespace CppTraining
{
template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept
{
  using std::swap;
  swap(lhs.size_, rhs.size_);
  swap(lhs.capacity_, rhs.capacity_);
  swap(lhs.data_, rhs.data_);
}

template <typename T>
Vector<T>::Vector(size_type capacity)
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

  // Without swap
  // if (this != &other)
  // {
  //   clear();
  //   shrink_to_fit();
  //   reserve(other.capacity_);

  //   for (const auto& value : other)
  //   {
  //     emplace_back(value);
  //   }
  // }

  // return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector&& other) noexcept
{
  swap(*this, other);

  return *this;

  // Without swap
  // if (this != &other)
  // {
  //   clear();
  //   shrink_to_fit();

  //   size_ = other.size_;
  //   capacity_ = other.capacity_;
  //   data_ = other.data_;

  //   other.size_ = 0_z;
  //   other.capacity_ = 0_z;
  //   other.data_ = nullptr;
  // }

  // return *this;
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
typename Vector<T>::size_type Vector<T>::size() const noexcept
{
  return size_;
}

template <typename T>
typename Vector<T>::size_type Vector<T>::capacity() const noexcept
{
  return capacity_;
}

template <typename T>
bool Vector<T>::empty() const noexcept
{
  return size_ == 0_z;
}

template <typename T>
typename Vector<T>::reference Vector<T>::front()
{
  return operator[](0_z);
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::front() const
{
  return operator[](0_z);
}

template <typename T>
typename Vector<T>::reference Vector<T>::back()
{
  return operator[](size_ - 1_z);
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::back() const
{
  return operator[](size_ - 1_z);
}

template <typename T>
typename Vector<T>::reference Vector<T>::at(size_type index)
{
  return operator[](index);
}

template <typename T>
typename Vector<T>::const_reference Vector<T>::at(size_type index) const
{
  return operator[](index);
}

template <typename T>
void Vector<T>::reserve(size_type new_capacity)
{
  if (new_capacity > capacity_)
  {
    // Think of ::operator new as C++'s version of malloc.
    // It allocates memory, but does not call constructors
    // so we need to call constructors manually.
    // We _could_ try to use realloc here, but it is not guaranteed
    // to work correctly with non-POD types.
    // Specifying the global new operator (::operator new) bypasses
    // any custom new operator overloads that might be defined
    auto new_data{reinterpret_cast<value_type*>(::operator new(new_capacity * sizeof(value_type)))};

    for (size_type i = 0_z; i < size_; ++i)
    {
      new (new_data + i) value_type(std::move(data_[i]));
      data_[i].~value_type();
    }

    // ::operator delete is the C++ version of free.
    // It deallocates memory, but does not call destructors.
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
void Vector<T>::push_back(const_reference value)
{
  if (size_ == capacity_)
  {
    reserve(capacity_ == 0_z ? 1_z : capacity_ * 2_z);
  }

  new (data_ + size_++) value_type(value);
}

template <typename T>
void Vector<T>::push_back(rvalue_reference value)
{
  if (size_ == capacity_)
  {
    reserve(capacity_ == 0_z ? 1_z : capacity_ * 2_z);
  }

  new (data_ + size_++) value_type(std::move(value));
}

template <typename T>
void Vector<T>::pop_back()
{
  if (size_ > 0)
  {
    data_[--size_].~value_type();
  }
}

} // namespace CppTraining
