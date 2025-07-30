# Lesson 1: Iterators, Allocators, and `emplace_back`

Welcome to Lesson 1 of the C++ Training Curriculum.

In this lesson, we evolve our custom `Vector<T>` to support **STL-style iterators**, **in-place construction with `emplace_back()`**, and **custom allocator behavior**.

---

### 🎯 Learning Objectives

By the end of this lesson, you will be able to:

- Implement **random-access iterators** with support for STL algorithms and range-based for loops
- Understand the difference between `Iterator` and `ConstIterator`
- Use `emplace_back()` with **perfect forwarding** to construct elements in-place
- Enable algorithms like `std::equal`, `std::sort`, and `std::copy_if`
- Understand the role of **allocators** and `std::allocator_traits`
- Handle **exception safety** in copy constructors (via `ThrowingCopy`)
- Support **custom growth policies** with function objects`

---

## 📁 Directory Overview

```text
lesson_1/
├── include/
│   ├── propogating_allocator.h # Custom allocator with propagation traits
│   ├── throwing_copy.h         # Type used for exception safety testing
│   ├── vector.h                 # Vector<T, Allocator> interface
│   ├── vector.inl               # Implementation details
└── tests/
    └── vector_test.cpp         # Extensive Catch2-based test suite
```

---

## 🧠 Key Features in This Lesson

### 🧵 Iterators (STL Compatibility)

Your `Vector<T>` now includes:

- `Vector<T>::Iterator` and `ConstIterator` types
- Bidirectional and random-access semantics
- Support for:
  - `begin()`, `end()`, `cbegin()`, `cend()`
  - `++it`, `--it`, `it + n`, `it - n`
  - `*it`, `it->`, `it[i]`
  - Comparisons and distance calculations

These iterators are STL-compatible and can be used with:

```cpp
for (auto& item : myVector) { ... }
std::sort(myVector.begin(), myVector.end());
std::copy_if(v.begin(), v.end(), ...);
```

---

### 🔨 `emplace_back()` and Perfect Forwarding

You’ll add:

```cpp
template <typename... Args>
Iterator emplace_back(Args&&... args);
```

- This enables **in-place construction**, avoiding unnecessary copies
- Utilizes `std::forward` and **variadic templates**
- Supports move-only types and late binding of constructor parameters

---

### 🧱 Allocator Support

Your `Vector<T>` now accepts a template parameter:

```cpp
template <typename T, typename Allocator = std::allocator<T>>
class Vector { ... };
```

- Memory is allocated/deallocated using `Allocator`
- Object construction/destruction is performed via `std::allocator_traits`
- Handles propagation rules (`propagate_on_container_*`)
- Provides allocator-aware `copy`, `move`, and `swap`

---

### 📈 Growth Policies

A `growth_policy_type` function allows flexible control over capacity behavior:

```cpp
Vector<T> v{0_z, std::allocator<T>{}, DefaultGrowthPolicy{}};
```

This allows you to experiment with fixed growth, doubling, or custom heuristics. You can pass any `std::function<size_t(size, capacity)>` to control this behavior.

---

### 🚨 Exception Safety with `ThrowingCopy`

The `ThrowingCopy` type simulates copy failures:

- Copy constructor throws when a global counter hits a target
- Useful for testing **strong exception guarantees** in constructors, `resize()`, and `push_back()`

---


## ✅ Summary

This lesson brings your custom `Vector<T>` significantly closer to STL-conformant behavior:

- You can now traverse and manipulate your container with fully-featured iterators
- Elements can be constructed directly in-place using `emplace_back()`, avoiding temporary objects
- Custom allocator support has been introduced, with full propagation semantics and trait-based integration
- Growth strategy is now configurable using user-defined policies
- Strong exception safety is validated through targeted tests

---
