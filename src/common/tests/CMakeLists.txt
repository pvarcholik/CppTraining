set(TARGET_NAME cpp_training_common_tests)

find_package(Catch2 CONFIG REQUIRED)

add_executable(${TARGET_NAME} foo_test.cpp)

target_link_libraries(${TARGET_NAME} PRIVATE cpp_training_common
                                             Catch2::Catch2WithMain)

include(Catch)
catch_discover_tests(${TARGET_NAME})
