
# C++ Training

The purpose of this training is to offer engineers a hands-on, progressive learning experience that builds both foundational and modern C++ proficiency through guided implementation challenges.

---

## 🎯 Goals

This curriculum is designed to:

- Strengthen understanding of C++ core language features, object lifetime, and templates
- Provide deep insight into memory management, RAII, and resource ownership
- Explore STL-like design by building reusable types from scratch
- Transition learners from C++14 to C++17 incrementally
- Reinforce the material found in the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
- Introduce useful tools for analyzing, testing, and improving your code

> **Note:** This series assumes familiarity with C++ basics (like functions and classes) but provides detailed guidance around idiomatic resource management and generic programming patterns.

---

## 👥 Intended Audience

This training is for engineers who:

- Already have a firm grasp of C++ syntax
- Have solid experience with C#, python, or another object-oriented language
- Want to go deeper into advanced C++ syntax and idioms


---

## 📚 Training Structure

Each lesson consists of:

- Source code for a type or small library
- Accompanying Catch2-based unit tests
- Code coverage tracking (via `lcov`)
- Progressive skill-building with real-world semantics and language mechanics

---

## 🧭 Lesson Overview

### ✅ Lesson 0 - Building `Vector<T>`

- Builds a custom `Vector<T>` container
- Explores manual memory management and RAII
- Introduces `placement new`, object lifetime, and move semantics
- Covers the Rule of 5 (copy/move construction/assignment and destructor)
- Provides robust Catch2-based tests in [`lesson_0/tests`](src/lesson_0/tests)
- Benchmarks behavior against STL concepts like `std::vector`

### 🧪 Lesson 1 - Iterators and `emplace_back` (Coming Soon)

- Adds iterators to support range-based for-loops and algorithms
- Introduces `emplace_back()` and perfect forwarding
- Compares value semantics to in-place construction
- Enables use of algorithms like `std::equal`
- Begins introducing C++17 capabilities (e.g. attributes, structured bindings)

---

## 🛠️ Getting Started

1. Clone the repository
   ```bash
   mkdir dev && cd dev
   git clone https://github.com/pvarcholik/CppTraining.git
   cd CppTraining
   ```
2. Install vcpkg ([additional details](https://github.com/microsoft/vcpkg)) and required dependencies:
   ```bash
   git clone https://github.com/Microsoft/vcpkg.git
   cd vcpkg
   ./bootstrap-vcpkg.sh
   vcpkg install ms-gsl catch2
   export VCPKG_ROOT=~/dev/vcpkg
   ```
   > **Note:** Consider adding VCPKG_ROOT to your `.bashrc` file
3. Install `lcov` for code coverage support
   ```bash
   sudo apt install lcov
   ```
4. Build and run from the command-line:
    ```bash
    cmake -B build -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
    cmake --build build
    ./build/src/common/tests/cpp_training_common_tests
    ./build/src/lesson_0/tests/cpp_training_lesson_0_tests
    ```
5. Generate code coverage reports:
    ```bash
    ./scripts/run_coverage.sh lesson_0
    ```
    This outputs *index.html* to `build/coverage/cpp_training_lesson_0_tests_html/lesson_0/include/index.html`
    Open that file in a browser to view code coverage details.
6. Build and run through VSCode:
    - Open the command palette
- Run tasks using:
- `Tasks: Run Build Task` (or `Ctrl+Shift+B`) and choose from the list:

  #### Available Tasks

  | Task                                      | Purpose                                                      |
  |-------------------------------------------|--------------------------------------------------------------|
  | **Generate All Lessons (DEBUG)**         | Configures the CMake project using the vcpkg toolchain       |
  | **Build All Lessons (DEBUG)**            | Builds all common and lesson modules                         |
  | **Run code coverage for selected targets** | Prompts for a lesson/test target, runs unit tests, and creates the HTML coverage report  |


## 🤝 Feedback and Contributions

This curriculum is built with longevity and extensibility in mind. If you're:

- Enjoying the material
- Finding gaps in the explanations
- Hitting issues during setup or while learning
- Or have ideas to improve the exercises

…please consider contributing or reaching out!

Ways to contribute:

- Open an internal issue or pull request
- Suggest enhancements to setup, code, or tests
- Propose new topics or improvements for upcoming lessons

Your feedback helps evolve this into a durable and flexible training path for modern C++ that grows with the team.
