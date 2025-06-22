# Lesson 0

- Introduction to template and container development
- Copy and move semantics
- Memory management and placement new

## Highlights
- What if T() throws?
  - new (data_ + i) value_type(some_throwing_ctor); // Can leak previously initialized value_type's
