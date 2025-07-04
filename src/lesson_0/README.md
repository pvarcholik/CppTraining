# Lesson 0: Building `Vector<T>`

Welcome to Lesson 0 of the C++ Training Curriculum.

In this lesson, you will build your own implementation of a generic `Vector<T>` container — similar in spirit to `std::vector`. This provides a hands-on environment for mastering **RAII**, **object lifetime**, and **manual memory management**; cornerstones of safe and performant C++.

---

## 🎯 Learning Objectives

By the end of this lesson, you will be able to:

- Understand how memory is allocated and tracked in dynamic containers
- Manually construct and destroy objects using `placement new` and `~T()`
- Implement constructors, destructors, and assignment operators (Rule of 5)
- Safely grow and shrink containers with `reserve`, `resize`, `shrink_to_fit`
- Write and run BDD-style unit tests using Catch2
- Generate line and function coverage reports with `lcov`

> You will also learn where (and why) `realloc()` doesn't work in modern C++, and how to manage raw memory safely using `::operator new` and `::operator delete`.

---

## 📁 Directory Overview

```text
lesson_0/
├── include/
│   └── vector.h        # Interface of Vector<T>
│   └── vector.inl      # Separated definition of Vector<T>
└── tests/
    └── vector_test.cpp # Catch2 BDD test suite
```

## 🔧 Key Features of `Vector<T>`

- Templates, standards-compliant type aliases and `type_traits`
- Capacity-managed storage (uninitialized memory)
- Explicit object construction using `placement new`
- Manual object destruction using `~T()`
- Element operations: `push_back`, `pop_back`, `resize`, `clear`, `shrink_to_fit`
- Bounds-checked access via `at()`
- Rule-of-5 support:
  - Copy constructor
  - Move constructor
  - Copy assignment
  - Move assignment
  - Destructor
- `swap()` support

## 💡 A Few Notable Concepts in This Lesson

### 📌 Notes on Type Aliases

The `Vector<T>` implementation includes a set of common type aliases:

```cpp
using size_type = std::size_t;
using value_type = T;
using reference = value_type&;
using const_reference = const value_type&;
using rvalue_reference = value_type&&;
```

#### Purpose

These aliases replicate conventions used by standard library containers like `std::vector`, and are crucial for:

- **Generic programming**
  They enable the use of standard algorithms (e.g., `std::equal`, `std::for_each`) and custom templates that expect consistent naming for member types.

- **STL compatibility**
  Many STL components (like iterators, traits, and algorithms) assume or require these alias names to be present in order to interact seamlessly with custom types.

### 📌 Notes on Type Traits

While those type aliases facilitate generic code, they are **not part of the type traits system**. The aliases are:

- Defined within the class
- Intended to describe container-specific types
- Used by templates to deduce properties without inspecting the internals of the container

Contrast this with **type traits**, which are separate utilities that perform **compile-time introspection**, like:

```cpp
std::is_trivially_copyable<T>
std::is_same<T, U>
std::enable_if<...>
```

C++ type traits, found in the `<type_traits>` header, provide compile-time information about types. They are an essential tool for writing **generic**, **type-safe**, and **optimized** code.

#### Common Type Traits

You’ll most often encounter type traits when writing templates or working with generic algorithms. Some of the most useful traits include:

```cpp
#include <type_traits>

static_assert(std::is_copy_constructible<Foo>::value, "Foo must be copy-constructible");
static_assert(std::is_trivially_copyable<Foo>::value, "Foo must be trivially copyable");
static_assert(std::is_move_assignable<Foo>::value, "Foo must be move assignable");
static_assert(std::is_default_constructible<Foo>::value, "Foo must be default constructible");
```

These checks run at **compile time** and will cause a compilation error if the assertion fails.

#### Tips
- Traits like `is_copy_constructible` or `is_move_assignable` are evaluated based on how the type is defined.
- A type is **not trivially copyable** if it defines its own copy/move constructors, destructor, or manages resources (like `new`/`delete`).
- In C++14, we use traits within `static_assert`, `enable_if`, and tag dispatching.



### `::operator new(size)` vs `new T(...)`

You’ll use:

```cpp
value_type* ptr = reinterpret_cast<value_type*>(::operator new(bytes));
```

This allocates raw memory **without constructing objects**. It's C++'s standards-compliant answer to `malloc()`, and it's safe to use for non-trivially constructible types when managing lifetime manually.

---

### `placement new`

This allows you to construct an object in memory you've already allocated:

```cpp
new (ptr + i) T(args...);
```

This is called **placement new**, and it's key to implementing STL-style containers and separates *allocation* from *construction*.

---

### Manual destruction

C++ does not implicitly manage lifetime for memory allocated with `::operator new`. You are responsible for calling the destructor explicitly:

```cpp
ptr->~T();
```

This is required to avoid memory/resource leaks and to trigger type-specific cleanup when you bypass `new T(...)`.

---

### Why not use `realloc()`?

While `realloc()` is fast and convenient in C, it's **not valid for C++ types that are non-trivially copyable**, such as types with constructors, destructors, or internal pointers (e.g. `std::string`). Using `realloc()` in those cases results in undefined behavior due to object lifetime violations.

Instead, this lesson teaches how to safely grow capacity:

1. Allocate new memory with `::operator new`
2. Move or copy old elements into the new location
3. Destroy the old elements
4. Deallocate the old memory with `::operator delete`

This mimics how `std::vector` works internally.

## 🛠️ Additional Core Language Concepts in This Lesson

While implementing `Vector<T>` we introduce a custom `Foo` type to demonstrate deep-copy semantics and to exercise our Vector<T> with a non-trivially copyable type. Along the way we also explore some foundational C++ concepts.

### Uniform Initialization (`{}` vs `()`)

This training uses **uniform initialization** with braces:

```cpp
Foo a{42}; // ✅ consistent, avoids narrowing conversion
```

We prefer this style because:

- It works with `std::initializer_list`
- Avoids implicit narrowing (e.g., truncating double to int)
- Helps eliminate the confusion between constructors and functional casts
- Works consistently for structs, aggregates, and classes

> 💡 Modern best practice is: prefer `{}` over `()` or `=` unless you have a reason not to.

---

### Type-Cast Constructors and the `explicit` Keyword

```cpp
explicit Foo(std::int32_t value);
```

By marking constructors with `explicit`, we prevent accidental implicit conversions like:

```cpp
Vector<Foo> v;
v.push_back(42); // ❌ Only valid if Foo(std::int32_t) were implicit
```

This safeguards type safety and readability — we disallow things the reader isn’t explicitly doing.

---

### noexcept

We mark move constructors and move assignment operators `noexcept`:

```cpp
Foo(Foo&& other) noexcept;
Foo& operator=(Foo&& other) noexcept;
```

This is critical for:

- STL compatibility
- Allowing `std::vector` and other containers to safely move objects during reallocation
- Signaling that move *cannot throw*, which enables optimizations and exception safety

---

### C++ Cast Operators (and using `reinterpret_cast` for `void*`)

In Lesson 0, we use:

```cpp
reinterpret_cast<T*>(::operator new(n * sizeof(T)));
```

We explain that:

- Both `static_cast<T*>` and `reinterpret_cast<T*>` are legal and equivalent for `void*`
- We chose `reinterpret_cast` for clarity — it’s **a low-level conversion**
- It reinforces that “this is a raw memory operation,” and not just a type-safe promotion

> ⚠️ Avoid `reinterpret_cast` when converting between unrelated types. It’s fine when used intentionally (as here) for memory operations.

---

### Data Layout and Virtual Function Tables

Using `Foo`, we discuss the implications of:

- Classes that have **a virtual destructor** (or any virtual functions)
- These classes hold a pointer to a **vtable** at runtime
- Reordering or removing virtual functions can change type layout
- All polymorphic types must be destroyed via their **dynamic type**

---

### Virtual Destructors and the `final` Keyword

```cpp
class Foo
{
public:
    virtual ~Foo(); // Required by convention
};
```

- If a type **is never meant to be inherited**, `final` means the compiler can optimize memory layout and function dispatch.
- If the type **is not marked final**, we must mark the destructor `virtual` to ensure safe polymorphic destruction.

> This prevents: `delete base; // calls base::~Base()` instead of `derived::~Derived()`!

---

### struct vs class

In C++, `struct` and `class` are the same except:

- `struct` = `public` members by default
- `class` = `private` members by default

At our company, we distinguish by purpose:

| Use `struct` for...         | Use `class` for...              |
|----------------------------|----------------------------------|
| Plain data aggregates       | Types with invariants and behavior |
| C-style layout or interop   | RAII, abstraction, and encapsulation |
| No constructors/destructors | Lifecycle-controlled objects       |
| Public data                 | Encapsulated/private data         |

We chose `class` for `Vector<T>` and `Foo` because:

- They manage resources (memory)
- They define lifetimes and ownership
- They encapsulate behavior, enforce invariants, and support the Rule of 5 — they are active types, not just passive data holders

> 💡 An invariant is the internal state a class guarantees will be consistent and valid, as long as clients interact through the public API.
---

## ✅ Summary

These foundational C++ topics may seem subtle, but they directly affect:

- How objects live and die
- What memory layout they take
- Whether constructors are chosen implicitly or explicitly
- Whether types can safely be used polymorphically

By the end of Lesson 0, you're building STL-style abstractions from scratch — with full control over semantics, allocation, and safety.

This hands-on experience provides the intuition needed to **apply the STL effectively, safely extend types**, and **write better production code**.
