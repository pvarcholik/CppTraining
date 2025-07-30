# Instructor's Notes — Lesson 1

This lesson introduced several C++ idioms — iterators, allocators, and `emplace_back()`. It also marks a transition: we're now writing code that resembles the STL not just in name or shape, but in actual behavior, compatibility, and design tradeoffs.

## Iterators

Our `Iterator` and `ConstIterator` types now satisfy the requirements for **random-access iterators** — including support for `operator+`, `operator-`, `operator[]`, and bidirectional traversal. This opens the door for full STL algorithm support, including `std::sort`, `std::equal`, and `std::copy_if`. Students should be able to use our custom vector in for-each loops and STL algorithms without any special workarounds.

One key deviation from `std::vector<T>::iterator` is the inclusion of a `container_` member. In standard containers, iterators typically do *not* track their parent container — they're just lightweight pointers or proxy objects. That design is efficient but it makes runtime validation impossible. Our iterators trade a bit of space and indirection for safety: we can detect and reject comparisons between iterators from unrelated containers, or accesses from default-constructed (invalid) iterators. This is especially useful for testing and debugging — and gives us a deeper appreciation for the STL’s performance-driven choices.

This lesson also gave us a nice opportunity to explore **const-correctness**, particularly around dereference semantics. Both `Iterator::operator*()` and `ConstIterator::operator*()` are marked `const`, but they behave differently:

- `ConstIterator::operator*()` returns a `const T&`
- `Iterator::operator*()` returns a `T&` — even though the operator itself is `const`

At first glance, that might seem contradictory.

> “How can the non-const iterator’s `operator*()` be `const`? Wouldn’t the compiler complain that we’re returning a non-const reference to something from within a `const` method?”

It’s a trick of indirection. The operator is `const` because it doesn’t modify the iterator object itself — it only accesses the container’s data via the internal `container_` pointer. Since `T&` is just a reference to the element in the container, there's no violation of const-correctness on the iterator.

This distinction between *const-iterator* and *iterator-constness* is subtle, but worth pointing out now — it shows up again in STL types and generic code.

## Allocators

We now accept an `Allocator` as a second template parameter and use `std::allocator_traits` throughout. Our `PropagatingAllocator<T>` demonstrates the propagation machinery that STL containers respect when copying, moving, or swapping.

I want to reinforce why this model is cumbersome — and why the STL's vector was redesigned in C++17 with `std::pmr::vector<T>`. The allocator-as-type-argument approach made generic code brittle, hard to read, and harder to evolve. `std::pmr::vector<T>` fixes this by abstracting allocation behind a runtime interface (`memory_resource`), decoupling allocation strategy from container instantiation. We'll circle back to this when we talk about dynamic polymorphism and type erasure.

For now, though, this design makes allocator propagation observable and testable. Students should come away understanding *why* allocator traits exist, how they're queried, and what implications they have on behavior during assignment or swap.

## Growth Policy Design

Unlike `Allocator`, the growth policy isn't part of the template signature — it's passed via constructor. This is deliberate. It lets you change growth behavior at runtime without creating new types. I think this is a great opportunity to contrast with how other STL types use type parameters to define behavior (e.g., `std::unordered_map<Key, T, Hash, KeyEqual>`).

It’s worth exploring the tradeoff: type parameters give you more compiler-time guarantees and possible optimizations, but they increase type complexity and reduce flexibility. Runtime composition (e.g., via `std::function` or lambdas) offers flexibility, at the cost of some runtime indirection and potential type erasure.

If time allows, I’ll connect this idea back to `std::function`, lambdas, and the idea of policy-based design.

---

### Follow-Up

This is a good point to encourage students to read — or re-read — Scott Meyers, Stephan T. Lavavej, or Howard Hinnant on allocator history. The STL allocator model is often mocked, but understanding it helps ground a lot of STL behavior that otherwise feels arbitrary.
