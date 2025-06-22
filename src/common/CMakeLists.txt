set(TARGET_NAME cpp_training_common)

find_package(Microsoft.GSL CONFIG REQUIRED)

if(MSVC)
  set(PLATFORM_OPTION WIN32)
endif()

add_library(${TARGET_NAME} STATIC ${PLATFORM_OPTION} src/foo.cpp)

target_include_directories(${TARGET_NAME}
                           PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(${TARGET_NAME} PUBLIC Microsoft.GSL::GSL)

target_compile_features(${TARGET_NAME} PUBLIC cxx_std_17)

add_subdirectory(tests)
