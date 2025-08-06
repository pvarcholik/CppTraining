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
│   ├── vector.h                # Vector<T, Allocator> interface
│   ├── vector.inl              # Implementation
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

#### 📚 What Is an Iterator?

An iterator is a generalization of a pointer — an object that allows you to traverse elements in a container, one at a time. Iterators support standard operations like advancing (`++it`), dereferencing (`*it`), comparison, and accessing elements by distance.

Iterators make container types compatible with standard algorithms in the STL, such as `std::sort`, `std::copy`, `std::find_if`, etc.

#### ⚙️ Iterator Categories

The STL defines a hierarchy of iterator capabilities:

- ➤ **ForwardIterator**: can move forward with `++it`, and supports comparison (`==`, `!=`). Useful for linear scans of single-pass containers like `std::forward_list`.

- ➤ **BidirectionalIterator**: adds support for moving backward via `--it`. Used by containers like `std::list` and `std::map`.

- ➤ **RandomAccessIterator**: supports full pointer-like semantics — `it + n`, `it - n`, `it[i]`, and relational comparisons (`<`, `>`). Required by algorithms such as `std::sort`.

Since `Vector<T>::Iterator` satisfies all of these requirements, it acts as a RandomAccessIterator enabling full compatibility with STL algorithms.

#### 🔒 ConstIterator — Read-Only Access

In addition to `Iterator`, your `Vector<T>` provides a `ConstIterator` type to support read-only traversal:

```cpp
ConstIterator it = myVector.cbegin();
```

A `ConstIterator` behaves just like a regular iterator but enforces that the elements it refers to cannot be modified. It returns a `const T&` when dereferenced, and only allows access to `const` member functions of the contained elements.

This ensures:
- STL algorithms like `std::for_each` or ranged `for` loops don’t accidentally mutate container contents
- You can safely expose iteration to client code without risking modification

Const iterators are automatically selected when iterating over a `const Vector<T>` (e.g., const reference or const pointer).

✅ Use `cbegin()` and `cend()` (const-begin and const-end) to explicitly ask for read-only access, through const iterators, even to containers that are mutable.

#### ⚠️ A note on the `typename` prefix

When you refer to a dependent name — that is, a name that depends on a template parameter — the compiler can't always determine whether it's a type or a value. For example:

```cpp
template <typename T>
void f() {
  typename T::value_type x; // OK: explicitly says this is a type
}
```

Without the `typename` keyword, the compiler would assume `T::value_type` is a static member variable or function (a value), not a type — leading to a compilation error.

This is why we must write:

```cpp
template <typename T, typename Allocator>
typename Vector<T, Allocator>::Iterator Vector<T, Allocator>::begin() noexcept
```

Here, `Vector<T, Allocator>::Iterator` is a dependent name — its meaning depends on the template parameters `T` and `Allocator`. So we have to tell the compiler explicitly: "this is a type."

✅ Use `typename` before any nested type name that depends on a template parameter when naming variables, return types, or using type aliases.

💡 Note: The need for typename is specific to dependent names. You don't need it when referring to known, non-dependent types — even inside a template.

For more context, this rule is part of C++'s **two-phase name lookup**: the compiler defers evaluation of certain names until template instantiation, but for parsing to succeed earlier, it needs enough information to disambiguate type vs. value.

---

### 🔨 `emplace_back()` and Perfect Forwarding

To support in-place construction of elements directly inside the vector's memory, `Vector<T>` now provides:

```cpp
template <typename... Args>
Iterator emplace_back(Args&&... args);
```

This section introduces several powerful modern C++ techniques: variadic templates, perfect forwarding, and placement new.

---

#### 🧳 Variadic Templates — Accepting “Any Number” of Arguments

A variadic template lets us accept an arbitrary number of function arguments of varying types. This is what enables `emplace_back()` to construct any `T` — regardless of how many constructor parameters it takes — right inside the vector:

```cpp
template <typename... Args>
Iterator emplace_back(Args&&... args);  // Accepts any number of arguments
```

Breaking this down:

- `typename... Args` — declares a parameter pack: a compile-time list of types
- `Args&&... args` — a forwarding reference pack that binds to both lvalues and rvalues
- You can think of it like: “This function can take 0 or more arguments, of any type, and forward them into the constructor of T”

This replaces the need for multiple overloads (e.g., one for `T(int)`, another for `T(std::string, double)`, etc.).

---

#### 🚀 Perfect Forwarding — Keeping lvalues/lvalues and rvalues/rvalues

When we call the constructor of `T`, we don’t want to blindly pass `args...`. That would copy every argument. Instead, we use:

```cpp
allocator_traits::construct(allocator_, data_ + size_, std::forward<Args>(args)...);
```

This uses `std::forward` to preserve the original value category and constness of each argument — letting us efficiently move when possible, and copy when necessary.

This enables idioms such as:

```cpp
Vector<std::string> v;
std::string name = "Alice";

v.emplace_back(name);             // Copy construct from lvalue
v.emplace_back(std::move(name));  // Move construct from rvalue
v.emplace_back(5, 'x');           // Construct string of 5 'x' characters
```

Behind the scenes, the template is perfectly forwarding the constructor arguments directly to `std::string`’s constructor.

---

#### 💡 Why Emplace and Not Just Push?

Whereas `push_back()` requires you to construct a `T` before inserting (i.e., incurs a copy or move), `emplace_back()` constructs the element directly in the allocated slot using placement new.

This is both faster and more flexible:

```cpp
v.push_back(Person{"Alice", 30});     // Construct + move into vector
v.emplace_back("Alice", 30);          // Construct in-place (no extra copies or moves)
```

Use `emplace_back()` by default when constructing objects with parameters. Prefer `push_back()` when you're inserting an already-constructed instance.

---

#### 📍 Summary

- `emplace_back()` provides in-place construction, reducing copies/moves
- It uses variadic templates to accept any constructor parameters
- It uses perfect forwarding (`std::forward`) to maintain argument fidelity
- It works with move-only types and types with many constructor signatures

You’ll see this idiom repeatedly in the STL — for example, in `std::map::emplace`, `std::tuple`, and `std::make_unique`.

---

### 🧱 Allocator Support

Your `Vector<T>` now accepts a custom allocator via its template parameter:

```cpp
template <typename T, typename Allocator = std::allocator<T>>
class Vector { ... };
```

This enables control over how memory is allocated and deallocated, making your vector compatible with user-defined memory strategies such as pooling, tracking, or arena allocation.

---

#### 🔧 What Are Allocator Traits?

Rather than calling allocator methods like `allocate()` and `deallocate()` directly, we use the standard utility:

```cpp
using allocator_traits = std::allocator_traits<Allocator>;
```

This layer abstracts over quirks in allocator interfaces and ensures portability across custom implementations. It provides static member functions, including:

- `allocate(allocator, object_count)` → call allocator’s `allocate` method
- `construct(allocator, ptr, args...)` → safely construct objects (usually via placement new)
- `destroy(allocator, ptr)` → safely destroy objects (calls destructor)
- Access to propagation rules (see next section)

Using traits ensures your container works seamlessly with both simple and advanced allocators.

---

#### 🏗 About `std::allocator<T>`

The default allocator, `std::allocator<T>`, uses `::operator new` and `::operator delete` behind the scenes. It does not carry state and is considered stateless across all containers. Therefore:

- All containers that use `std::allocator<T>` will consider any two allocator instances equal
- Copies and swaps involving `std::allocator<T>` are trivial
- Propagation traits default to `false_type` — allocator is not copied or moved because all instances behave identically

In other words: unless you've replaced the default with a stateful allocator, allocator propagation has no visible effect when using `std::allocator<T>`.

---

#### 🔄 What Is Allocator Propagation?

When your container is copied, moved, or swapped, a key design question emerges:

> Should the allocator instance be copied, moved, or swapped as well?

This is governed by traits in `std::allocator_traits`:

| Trait                                    | Effect if `true_type`                       |
|-----------------------------------------|---------------------------------------------|
| `propagate_on_container_copy_assignment`| Copy assignment also copies allocator       |
| `propagate_on_container_move_assignment`| Move assignment adopts allocator from source|
| `propagate_on_container_swap`           | Swap includes allocator state               |

These are opt-in behaviors — only stateful allocators (like `PropagatingAllocator<T>`) typically customize them.

If your allocator doesn’t explicitly define these traits, `std::allocator_traits` supplies default values:

```cpp
propagate_on_container_copy_assignment   = std::false_type
propagate_on_container_move_assignment   = std::false_type
propagate_on_container_swap              = std::false_type
```

This means that if allocator propagation is not requested:
- The allocator will not be replaced during assignment or swap
- Containers must preserve their own allocator
- Deep copies will be used instead of cheap moves

---

#### 🧪 Example: Using a Propagating Allocator

```cpp
Vector<Foo, PropagatingAllocator<Foo>> v1{0_z, PropagatingAllocator<Foo>{1}};
Vector<Foo, PropagatingAllocator<Foo>> v2{0_z, PropagatingAllocator<Foo>{2}};

v1 = v2;
// If propagate_on_container_copy_assignment == true:
//   v1's allocator is replaced with v2's
// If it’s false and allocators differ:
//   existing allocator is used to reallocate contents
```

---

#### 💡 Summary

- `std::allocator<T>` is the default and stateless — simple and fast
- `allocator_traits` helps generic code adapt to any allocator
- Propagation traits influence assignment and swap behavior
- Custom allocators (like `PropagatingAllocator<T>`) let you observe or control these interactions

---
### 📈 Growth Policy

Your `Vector<T>` now supports configurable growth behavior via a user-defined policy passed at construction time:

```cpp
Vector<T> v{0_z, std::allocator<T>{}, DefaultGrowthPolicy{}};
```

The growth policy is a callable (like a lambda, functor, or function pointer) of the form:

```cpp
std::size_t growth_rule(std::size_t current_size, std::size_t current_capacity);
```

This allows you to control how the vector's capacity grows when elements are added — for example, doubling on each resize, growing linearly, or staying fixed for benchmarking.

---

#### 🤔 Why Not a Template Parameter?

Unlike `Allocator`, which is part of the `Vector<T, Allocator>` template definition, the growth policy is passed at runtime via the constructor. This has two important consequences:

1. ✅ You can choose or change growth policies without needing to instantiate new types.
2. 🔧 The runtime flexibility allows injection of lambdas or stateful functors for experimentation and diagnostics.

This is a contrast with many STL containers that use template parameters for behavioral policies. For example:

```cpp
std::unordered_map<Key, T, HashFunction, EqualityPredicate>
```

That compile-time approach offers strong optimization potential, but can make types harder to reuse or abstract over.

---

#### ⚖️ Tradeoffs: Type vs. Runtime Configuration

| Approach            | Pros                                   | Cons                                      |
|---------------------|----------------------------------------|-------------------------------------------|
| Template Policy     | Compile-time guarantees, inlining, performance | Inflexible; new type for each variation   |
| Runtime Policy      | Flexible, testable, configurable       | Slight overhead; less optimization        |

In our training vector, we prioritize simplicity and runtime configurability by using `std::function<size_t(size, capacity)>`. This keeps the implementation modular and easy to experiment with.

> 💡 Tip: This pattern (injecting runtime behavior via callable objects) is common in policy-based design, type erasure, and even modern STL features like `std::pmr`.

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

---
