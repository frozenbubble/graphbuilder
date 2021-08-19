cmake_minimum_required(VERSION 3.14)
project(graphbuilder)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_library(graphbuilder graph.cc graphbuilder.cc)
add_executable(graphman manualtest.cc graph.cc graphbuilder.cc)
add_executable(graph_test graph_test.cc graph.cc)
target_link_libraries(
  graph_test
  gtest_main
  )
target_include_directories(graph_test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

include(GoogleTest)
gtest_discover_tests(graph_test)