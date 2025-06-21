#include "vector.h"

namespace CppTraining
{
template <typename T>
void swap(Vector<T>& lhs, Vector<T>& rhs) noexcept
{
  std::swap(lhs.size_, rhs.size_);
  std::swap(lhs.capacity_, rhs.capacity_);
  std::swap(lhs.data_, rhs.data_);
}

template <typename T>
Vector<T>::Vector(size_type capacity)
{
  reserve(capacity);
}

template <typename T>
Vector<T>::Vector(const Vector& other)
  : Vector{other.capacity_}
{
  if (capacity_ > 0_z)
  {
    for (size_type i = 0_z; i < other.size_; ++i)
    {
      new (data_ + i)value_type(other.data_[i]);
    }

    size_ = other.size_;
  }
}

template <typename T>
Vector<T>::Vector(Vector&& other) noexcept
  : size_{other.size_}
  , capacity_{other.capacity_}
  , data_{other.data_}
{
  other.size_ = 0_z;
  other.capacity_ = 0_z;
  other.data_ = nullptr;
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& other)
{
  auto copy{ other };
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
Vector<T>::~Vector()
{
  for (size_type i = 0_z; i < size_; ++i)
  {
    data_[i].~value_type();
  }

  free(data_);
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
void Vector<T>::reserve(size_type capacity)
{
  if (capacity > capacity_)
  {
    auto data{reinterpret_cast<value_type*>(realloc(data_, capacity * sizeof(value_type)))};
    assert(data != nullptr && "Memory allocation failed");
    data_ = data;
    capacity_ = capacity;
  }
}

template <typename T>
void Vector<T>::resize(size_type size)
{
  if (size < size_)
		{
			for (size_type i = size; i < size_; ++i)
			{
				data_[i].~value_type();
			}

			if (size == 0_z)
			{
				free(data_);
				data_ = nullptr;
			}
			else
			{
				auto data{reinterpret_cast<value_type*>(realloc(data_, size * sizeof(value_type)))};
				 assert(data != nullptr && "Memory allocation failed");
        data_ = data;
				capacity_ = size_ = size;
			}
		}
  else
  {
    reserve(size);
    for (size_type i = size_; i < size; ++i)
    {
      new (data_ + i)value_type{};
    }

    size_ = size;
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
    free(data_);
    data_ = nullptr;
    capacity_ = 0;
  }
  else if (size_ != capacity_)
  {
    auto data{reinterpret_cast<value_type*>(realloc(data_, size_ * sizeof(value_type)))};
    assert(data != nullptr && "Memory allocation failed");
    data_ = data;
    capacity_ = size_;
  }
}

} // namespace CppTraining
