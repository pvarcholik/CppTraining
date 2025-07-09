set(TARGET_NAME cpp_training_lesson_0_tests)

add_executable(${TARGET_NAME} vector_test.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE cpp_training_lesson_0
                                             Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(${TARGET_NAME})
