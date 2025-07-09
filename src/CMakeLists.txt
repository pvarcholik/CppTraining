cmake_minimum_required(VERSION 3.14)

include(FetchContent)

FetchContent_Declare(GSL
    GIT_REPOSITORY "https://github.com/microsoft/GSL"
    GIT_TAG "v4.2.0"
    GIT_SHALLOW ON
)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.8.1
)

FetchContent_MakeAvailable(GSL)

FetchContent_MakeAvailable(Catch2)

add_subdirectory(common)
add_subdirectory(lesson_0)
add_subdirectory(lesson_1)
