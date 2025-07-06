#pragma once

namespace CppTraining
{
template <typename T>
class PropagatingAllocator final
{
public:
  using value_type = T;
  using propagate_on_container_swap = std::true_type;
  using propagate_on_container_copy_assignment = std::true_type;
  using is_always_equal = std::false_type;

  explicit PropagatingAllocator(std::uint32_t id = 0) : id_{id} {};
  template <class U>
  PropagatingAllocator(const PropagatingAllocator<U>&) noexcept
  {
  }
  PropagatingAllocator(const PropagatingAllocator&) = default;
  PropagatingAllocator(PropagatingAllocator&&) noexcept = default;
  PropagatingAllocator& operator=(const PropagatingAllocator&) = default;
  PropagatingAllocator& operator=(PropagatingAllocator&&) noexcept = default;
  ~PropagatingAllocator() = default;

  T* allocate(std::size_t n) { return reinterpret_cast<T*>(::operator new(n * sizeof(T))); }

  void deallocate(T* p, std::size_t) noexcept { ::operator delete(p); }

  std::uint32_t getId() const { return id_; }

private:
  std::uint32_t id_;
};

template <typename T, typename U>
bool operator==(const PropagatingAllocator<T>& lhs, const PropagatingAllocator<U>& rhs) noexcept
{
  return lhs.getId() == rhs.getId();
}

template <typename T, typename U>
bool operator!=(const PropagatingAllocator<T>& lhs, const PropagatingAllocator<U>& rhs) noexcept
{
  return !(lhs == rhs);
}
} // namespace CppTraining
