# Lesson 0: Building `Vector<T>`

Welcome to Lesson 0 of the C++ Training Curriculum.

In this lesson, you will build your own implementation of a generic `Vector<T>` container — similar in spirit to `std::vector`. This provides a hands-on environment for mastering **RAII**, **object lifetime**, and **manual memory management**; cornerstones of safe and performant C++.

---

### 🎯 Learning Objectives

By the end of this lesson, you will be able to:

- Implement a minimal `Vector<T>`-like container with support for dynamic resizing and deep-copy semantics
- Apply the **Rule of Five** to user-defined types (copy/move constructors, assignment, destructor)
- Allocate and manage raw memory safely using `::operator new` / `operator delete`
- Construct and destroy objects manually using **placement new** and explicit destructor calls
- Identify and use **type aliases** and **type traits** to enable generic programming and trait-based enforcement
- Use the **copy-swap idiom** to implement exception-safe assignment operators
- Demonstrate understanding of **virtual destructors**, vtables, and dynamic object destruction semantics
- Write and run unit tests using Catch2, and generate basic coverage reports via `lcov`

> 💬 This lesson lays the foundation for deep, idiomatic C++ by introducing the mechanics behind container interfaces, type safety, and memory control.

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

- Templates and standards-compliant type aliases
- Capacity-managed storage (uninitialized memory)
- Explicit object construction using `placement new`
- Manual object destruction using `~T()`
- Element operations: `push_back`, `pop_back`, `resize`, `clear`, `shrink_to_fit`
- Bounds-checked access via `at()` (but maybe also `operator[]`)
- Rule-of-5 support:
  - Copy constructor
  - Move constructor
  - Copy assignment
  - Move assignment
  - Destructor
- `swap()` support

## 💡 A Few Notable Concepts in This Lesson

### 📌 Type Aliases

The `Vector<T>` implementation includes a set of common type aliases:

```cpp
using size_type = std::size_t;
using value_type = T;
using reference = value_type&;
using const_reference = const value_type&;
using rvalue_reference = value_type&&;
```

These aliases replicate conventions used by standard library containers like `std::vector`, and are crucial for:

- **Generic programming**
  They enable the use of standard algorithms (e.g., `std::equal`, `std::for_each`) and custom templates that expect consistent naming for member types.

- **STL compatibility**
  Many STL components (like iterators, traits, and algorithms) assume or require these alias names to be present in order to interact seamlessly with custom types.

### 💡 Sidebar: lvalues, rvalues, and rvalue references

In C++, a key innovation of modern (C++11+) language design is the support for **move semantics**, and it hinges entirely on how the language distinguishes *lvalues and rvalues*.

#### 📎 Basic definitions

| Expression          | Treated as... | Meaning                                               |
|---------------------|----------------|--------------------------------------------------------|
| `T x = 42;`         | lvalue        | Named, addressable object                             |
| `x`                 | lvalue        | Still an lvalue when accessed later                  |
| `42`                | rvalue        | Temporary value (you can't take its address)         |
| `T{}`               | rvalue        | Temporary result of a constructor                    |
| `std::move(x)`      | rvalue        | *Explicitly* converted to an rvalue for moving       |

#### ⚙️ Type categories

- `T&`: lvalue reference — binds *only* to lvalues
- `T&&`: rvalue reference — binds *only* to rvalues
  ⚠️ (except in template deduction — see "*forwarding references*")

---

### 🛠 Why `push_back` Uses an Rvalue Reference Overload

To support efficient insertion, we define two overloads:

```cpp
void push_back(const_reference value);  // binds lvalues; usually results in a copy
void push_back(rvalue_reference value); // binds rvalues; usually results in a move
```

- The first binds to named/lvalue objects (**non-temporaries**) and internally invokes the copy constructor of `T`
- The second binds to rvalues (**temporaries or moved-from objects**) and internally invokes the move constructor of `T`

This lets you write:

```cpp
Vector<Foo> v;
Foo a{42};

v.push_back(a);             // Calls lvalue overload → copy
v.push_back(std::move(a));  // Calls rvalue overload → move
v.push_back(Foo{13});       // Calls rvalue overload → move (temporary)
```

---

### ⚠️ Without the rvalue overload:

If you only define `push_back(const T&)`, then:
- All pushes are **copies**, even if the source is a temporary.
- Performance suffers if `T` is expensive to copy.

So we use `push_back(T&&)` to give temporaries (rvalues) their **most efficient path** — a move.

---

### 📌 Type Traits

While type aliases facilitate generic code, they are **not part of the type traits system**. The aliases are:

- Defined within the class
- Intended to describe container-specific types
- Used by templates to deduce properties without inspecting the internals of the container

Contrast this with **type traits**, which are separate utilities that perform **compile-time introspection**, like:

```cpp
std::is_trivially_copyable<T>
std::is_same<T, U>
std::enable_if<...>
```

C++ type traits, found in the `<type_traits>` header, provide compile-time information about types.

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

### 🧱 Allocation, Construction, and Destruction in C++

Modern C++ gives us low-level control over memory and object lifetimes by separating:

- **Memory allocation** (`operator new`)
- **Object construction** (`placement new`)
- **Object destruction** (explicit destructor calls)
- **Memory deallocation** (`operator delete`)

This is the foundation of how types like `std::vector<T>` manage their internal storage.

---

### `new` / `delete` Expressions vs. Operators

|         | **Expression**          | **Operator**                   |
|---------|--------------------------|--------------------------------|
| Allocating | `new T(args...)`         | `::operator new(size)`         |
| Deallocating | `delete ptr`            | `::operator delete(ptr)`        |
| Constructs object? | ✅ Yes (calls constructor) | ❌ No (raw memory)               |
| Calls destructor? | ✅ Yes (when deleting) | ❌ No (manual cleanup required) |

#### 💡 Summary:

- **Expressions**:
  - `new T(args...)`: allocates + constructs
  - `delete ptr`: calls destructor + deallocates

- **Operators**:
  - `::operator new(size)`: just allocates raw memory
  - `::operator delete(ptr)`: just deallocates raw memory

These operators can also be **customized** or **overloaded** per class.

---

### `::operator new(size)` – Raw allocation

To manually allocate memory for `n` objects of type `T`, use:

```cpp
std::size_t bytes = n * sizeof(T);
value_type* ptr = reinterpret_cast<value_type*>(::operator new(bytes));
```

This is like C's `malloc`, but it guarantees proper alignment and interacts correctly with constructors/destructors. No objects are constructed yet.

---

### **Placement new** – Constructing in pre-allocated memory

To construct an object at a specific address:

```cpp
new (ptr + i) T(args...);
```

- This syntax is called **placement new**
- It constructs a new `T` **in-place** in memory you allocated separately
- You are now responsible for managing that object’s lifetime

🚫 Using the regular `new T(...)` here would **allocate new memory**, which is not what you want.

### 😬 Sidebar: `new[]`, `delete[]`, and the Footguns of C++

C++ has two families of memory management functions:

- `new` / `delete`
- `new[]` / `delete[]`

These are distinct and **not interchangeable**:

```cpp
int* a = new int[10];
delete a;      // ❌ UB! Should be delete[]

int* b = new int;
delete[] b;    // ❌ Also UB
```

### 🔥 Why Do We Have `delete[]`?

When you use `new[]`, the compiler may insert *hidden metadata* (like the number of elements) before the start of the block. This allows `delete[]` to call destructors on each element.

> 💭 But... this memory layout is not standardized. Compilers do this differently.

### 🤔 So What’s the Problem?

- `delete[]` must match a `new[]` **exactly**
- Failing to do so leads to **undefined behavior**
- There's no way to detect mismatches at runtime (!)
- In practice, people often confuse them (especially in legacy code)

💬 **Herb Sutter** and others have argued that `delete[]` is a design anomaly:
> "Wouldn't it be nice if arrays didn't need their own special form of new/delete?"

It's one more reason modern C++ encourages:
- `std::vector<T>` for dynamic-sized arrays
- `std::unique_ptr<T[]>` only when absolutely necessary
- Avoiding manual `new[]` and `delete[]` entirely

---

### 🧠 Moral of the sidebar

Unless you're writing a lesson like this one or building an allocator:
> 🚫 Don't use `new[]`/`delete[]`. Use `std::vector<T>` or `std::make_unique<T[]>` instead.

They’re safer, clearer, and less footgun-prone.

---

### **Manual destruction**

Since placement new doesn’t track objects, you must explicitly call the destructor for each element:

```cpp
(ptr + i)->~T();
```

This is especially important for types that manage their own resources (`std::string`, `std::vector`, etc.).

---

### Why not use `realloc()`?

C’s `realloc()` is dangerous in C++ for non-trivial types:

- It can move memory **without calling constructors or destructors**
- This violates C++ object lifetime rules (undefined behavior)
- It’s only safe for **trivially copyable** types (like `int`, `char`, or PODs)

Instead, our pattern is:

1. Allocate new memory with `::operator new`
2. Copy/move-construct into new storage (placement new)
3. Manually destroy old elements
4. Deallocate old memory with `::operator delete`

This mimics how `std::vector` grows its backing buffer.

---

### 🔍 Sidebar: Virtual Destructors and VTable Entries

When a class has a **virtual destructor**, the compiler may emit **two separate vtable entries** for the destructor:

1. Complete destructor: `Foo::~Foo()`
2. Deleting destructor: implicitly generated — calls complete destructor and frees memory

This supports situations like:

```cpp
delete basePtr; // basePtr has dynamic type Foo*
```

And internally:

- The **deleting destructor** calls the complete destructor, then calls `operator delete(this)`
- This distinction is needed for correctness in class hierarchies and **placement delete**

In practice, most developers only need to know:

- If your class is polymorphic (i.e., has virtual functions), you should **declare a virtual destructor**
- This ensures `delete` works safely through a pointer to base

---

## 🛠️ Additional Core Language Concepts in This Lesson

While implementing `Vector<T>` we introduce a custom `Foo` type to demonstrate deep-copy semantics and to exercise our Vector<T> with a non-trivially copyable type. Along the way we also explore some foundational C++ concepts.

### Uniform Initialization (`{}` vs `()`)

This training uses **uniform initialization** with braces:

```cpp
Foo a{42};    // ✅ consistent, avoids narrowing conversion such as:
int x = 3.14; // OK (truncates)
int y{3.14};  // ERROR: narrowing
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

### 🔄 Copy-Swap Idiom

The **copy-swap idiom** is a robust and exception-safe way to implement the copy assignment operator and, optionally, the move assignment operator.

The key idea is to implement assignment in terms of **copy construction + `swap()`**, leveraging the already-correct copy constructor and making the operation exception-safe.

---

#### 🧱 Structure

```cpp
class Foo {
public:
    Foo(const Foo& other);        // copy constructor
    void swap(Foo& other) noexcept;  // swap method

    // copy assignment via copy-swap
    Foo& operator=(Foo other)
    {
      swap(other);
      return *this;
    }
};
```

Yes — the copy assignment operator takes its parameter **by value** (i.e., a local copy). That local instance may be initialized via copy or move constructor, depending on how the function is invoked.

---

### 🔍 Why It Works

1. You copy (or move) from the argument into a local object
2. You `swap(*this, other);` to replace the current object's contents
3. The destructor tears down the old contents automatically (in `other`)

This effectively isolates all mutation until after successful construction — making it **exception-safe** by design.

---

### ✅ Benefits

- **Strong exception guarantee**: state is not modified if anything fails
- **Code reuse**: logic lives in copy constructor and `swap()` only
- **Often trivial to implement** when `swap()` is already needed

---

### ⚠️ Drawbacks / Considerations

- May involve **extra copy or move**, depending on compiler optimizations
- May not be appropriate for performance-critical inner loops
- You need a valid and efficient `swap()` which should be marked noexcept

For maximum performance, the move assignment operator may need a hand-written implementation instead of copy-swap.

---

### 🧪 Example: Implementing `swap`

```cpp
void Foo::swap(Foo& other) noexcept
{
  using std::swap;
  swap(data_, other.data_);
}
```

You can also define a non-member `swap(Foo&, Foo&)` to enable Argument-Dependent Lookup (ADL) and make it compatible with `std::swap`.

#### 🧠 ADL — Argument-Dependent Lookup

When calling a function like `swap(a, b)` without prefixing it with `std::`, the compiler checks:

1. The current scope
2. The **namespace of the types** of `a` and `b`

This rule is known as **Argument-Dependent Lookup** (ADL). It allows custom `swap()` implementations defined in the same namespace as your type to be automatically found and used.

##### ✅ Best Practice

When writing generic code:

```cpp
using std::swap;
swap(a, b);  // ADL-friendly: uses your swap if available, otherwise std::swap
```

This ensures better performance for classes that define a more efficient `swap()`.

> 💡 Note: In `Vector<T>`, all members are native types, so there's no custom `swap()` to be found via ADL. In this case, `std::swap(...)` behaves the same as `swap(...)` with `using std::swap;`. However, using the ADL-friendly form is a good habit and keeps your code consistent with STL conventions.

---

#### ✅ Mark `swap()` as `noexcept` When Safe

If your type's `swap()` cannot throw, mark it `noexcept`:

```cpp
void swap(Foo& other) noexcept
{
    using std::swap;
    swap(data_, other.data_);
}
```

Reasons to do this:

- STL containers (like `std::vector`) prefer move/swap operations **only if marked `noexcept`**
- Improves exception-safety guarantees
- Enables optimizations like move-based reallocation

Thus, combining ADL with `noexcept` creates both **correct** and **efficient** code.

---

### 📚 Additional Reading

- [C++ Core Guidelines: C.127 — Use the copy-and-swap idiom for copy assignment](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rc-swap)
- Meyers, *Effective C++*, Item 11

```
Note: In C++11 and later, move assignment is often implemented separately for stronger performance.
```

## ✅ Summary

These foundational C++ topics may seem subtle, but they directly affect:

- How objects live and die
- What memory layout they take
- Whether constructors are chosen implicitly or explicitly
- Whether types can safely be used polymorphically

By the end of Lesson 0, you're building STL-style abstractions from scratch — with full control over semantics, allocation, and safety.

This hands-on experience provides the intuition needed to **apply the STL effectively, safely extend types**, and **write better production code**.

---

### 🚀 Coming Up in Lesson 1: Iterators, STL Compatibility, and Allocators (C++14)

In Lesson 0, you built a working `Vector<T>` using manual memory management and value semantics.

In Lesson 1, we will take a big step forward by enabling compatibility with the **Standard Template Library (STL)** and supporting **custom allocator behavior**.

You’ll:

✅ **Implement bidirectional and random-access iterators**, including:

- `Vector<T>::Iterator` and `ConstIterator`
- Support for `begin()`, `end()`, `cbegin()`, `cend()`
- Iterator operations: `*it`, `it->`, `++it`, `--it`, `it + n`, `it - n`, `it[i]`, and distance calculations

✅ **Write exception-safe and STL-compliant memory management**, including:

- Use of `std::allocator_traits`
- Support for custom allocators via a second template parameter
- Proper delegation of `allocate`, `deallocate`, `construct`, and `destroy`

✅ **Add allocator propagation support**, including:

- Traits like `propagate_on_container_copy_assignment`
- Correct behavior for move assignment and swap based on allocator compatibility

✅ **Enable use of STL algorithms**, such as:

```cpp
for (auto& elem : myVector) { ... }
std::sort(myVector.begin(), myVector.end());
std::copy_if(myVector.begin(), myVector.end(), ...);
```

This lesson sets the foundation for building STL-compatible data structures while giving you an inside look at how real containers support allocation strategies and iterator semantics.

---
