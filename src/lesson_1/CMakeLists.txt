set(TARGET_NAME cpp_training_lesson_1)

if(MSVC)
  set(PLATFORM_OPTION WIN32)
endif()

add_library(${TARGET_NAME} ${PLATFORM_OPTION} INTERFACE)

target_include_directories(${TARGET_NAME}
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${TARGET_NAME} INTERFACE cpp_training_common)

add_subdirectory(tests)
