## 🎤 Instructor’s Notes: Lesson 0

### 🌟 Introduction

- Set the stage: we're building a simplified `Vector<T>` to understand object lifetime, memory management, and container internals
- Position the work: building a vector is not about reinventing `std::vector`, it’s about deeply learning the mechanics underlying safe and efficient C++
- Highlight what's gained: better STL usage, correct RAII, and clearer mental models of object lifetimes

---

### 📗 Type Aliases + Traits

- Start with the `Vector<T>` interface and walk through the standard container-style type aliases:
  - `value_type`, `size_type`, `reference`, `rvalue_reference`, etc.
  - Review lvalues, rvalues, and lvalue and rvalue references
- Justify their role in enabling STL interop and clean template code
- Introduce type traits (`<type_traits>`) and their use in constraint enforcement
  - `static_assert(std::is_copy_constructible<T>::value)`
- Discuss how traits like `is_trivially_copyable` intersect with real bugs (e.g., misuse with `realloc`)

---

### 🔁 Copy & Move Semantics (Introduce `Foo`)

- Introduce the `Foo` class and use it to motivate discussions on resource ownership
- Walk through:
  - Copy constructor: performs a deep copy of an `int*`
  - Move constructor: takes ownership, nulls source pointer
  - Destructor: deletes `data_`
- Show copy/move assignment operators and why self-assignment checks matter
- Explain "why not just always copy?" → performance, correctness when managing resources
- Highlight:
  - What makes a type **trivially copyable**
  - What calling `std::move()` actually does — converts an lvalue to an rvalue
- Optional: Live-demo or log from `Foo` to inspect copy/move calls

---

### 🧹 Rule of Five + Copy-Swap Idiom

- Define the Rule of Five:
  - Copy constructor, move constructor
  - Copy assignment, move assignment
  - Destructor
- Explain how managing resources (e.g., owning a pointer) forces the need to implement all five
- Implement `operator=(Vector<T> other)` using the **copy-swap idiom**
  - Emphasize exception-safety
  - Show how the idiom cleanly reuses constructors + `swap()`
- Reinforce: while copy-swap is elegant, STL types often use hand-optimized assignments for performance-critical paths

---

### 🧱 Uniform Initialization

- Show example: `Foo a{42};` vs `Foo b(42);` vs `Foo c = 42;`
- Explain how brace initialization has key benefits:
  - Prevents narrowing (`int x{3.14}` fails)
  - Works with aggregate types, `std::initializer_list`
  - Eliminates subtle overload ambiguities
- Encourage using `{}` as default initialization style unless there's a strong reason not to

---

### 🔐 Explicit Constructors

- After showing uniform initialization, explain why single-argument constructors should often be marked `explicit`
- Show how dropping `explicit` allows unintended conversions:
  ```cpp
  Vector<Foo> v;
  v.push_back(42); // ✓ if Foo(int) is implicit; ✗ if it's explicit
  ```
- Emphasize:
  - Avoiding magical conversions
  - Cleaner, more intentional API usage
  - Most constructors taking a single parameter should be `explicit` unless they're meant to act like implicit type conversions

---

### ⚙️ Manual Memory Management

- Emphasize allocation/construction separation:
  - `::operator new(n * sizeof(T))` allocates raw bytes
  - `placement new(ptr) T(...)` constructs objects
- Show how and why to explicitly destroy objects: `ptr->~T()`
- Relate it directly to how `Vector::push_back()` and `Vector::reserve()` handle element lifetime
- Highlight `std::vector` as a higher-level abstraction that does this under the hood

---

### 🛑 Sidebar – Dangers of `new[]` / `delete[]`

- Explain the mismatch risk: `new[]` must be paired with `delete[]`, `new` with `delete`
- Describe how many compilers allocate metadata for `new[]`, making the layout non-standard
- Share Herb Sutter’s observation: it's easy to misuse and hard to test for
- Point learners toward `std::vector` or `std::make_unique<T[]>` for safer alternatives
- Reaffirm: `Vector<T>` uses `new` + `placement new` + manual destruction instead — deliberately avoids `new[]`

---

### 🔄 Move Semantics in `push_back`

- Reuse knowledge from `Foo` to explain how `Vector<T>::push_back` is optimized
- Define lvalues vs rvalues, and refresh reference types:
  - `T&` binds to lvalues
  - `T&&` binds to rvalues (temporaries and `std::move`)
- Show dual overloads for `push_back`:
  ```cpp
  void push_back(const T&);  // copy
  void push_back(T&&);       // move
  ```
- Explain:
  - Why `std::move` is required to invoke the rvalue overload
  - How omitting the rvalue version causes *all* `push_back` operations to use copy
- Optionally demonstrate with `Foo`: log copy vs. move constructor usage during `push_back(a)` vs `push_back(std::move(a))`

---
