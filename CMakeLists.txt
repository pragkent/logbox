cmake_minimum_required(VERSION 2.6)

PROJECT(logbox)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include(CheckCXXCompilerFlag)

# __builtin_expect
check_cxx_source_compiles("
  int main() {
    if(__builtin_expect(1<2, 1)) {}
    return 0;
  }
" HAVE_BUILTIN_EXPECT)

# __attribute__ ((noinline))
check_cxx_source_compiles("
  void foo() __attribute__((noinline));
  int main() {
    return 0;
  }
" HAVE_ATTRIBUTE_NOINLINE)

# __attribute__ ((noreturn))
check_cxx_source_compiles("
  void foo() __attribute__((noreturn));
  int main() {
    return 0;
  }
" HAVE_ATTRIBUTE_NORETURN)

set(logbox_SOURCES
  src/core/logging.cc
  src/core/sink_control.cc
  src/core/context.cc
  src/core/logging_event.cc
  src/core/log_severity.cc
  src/core/format.cc
  src/sink/console_sink.cc
  src/sink/rolling_file_sink.cc
  src/util/environment_posix.cc
  src/util/posix_error.cc
  src/util/timestamp.cc
  src/util/writable_file.cc
  )

set(logbox_HEADER_DIR ${PROJECT_SOURCE_DIR}/src/include/logbox)

configure_file(cmake/feature_toggle.h.cmake ${logbox_HEADER_DIR}/feature_toggle.h)

include_directories(src/include src gmock)

add_library(logbox ${logbox_SOURCES})
target_link_libraries(logbox "pthread")
set_target_properties(logbox PROPERTIES COMPILE_FLAGS
  "-Wall -Wextra -pedantic -std=c++11")

# We compile Google Test ourselves instead of using pre-compiled libraries.
# See the Google Test FAQ "Why is it not recommended to install a
# pre-compiled copy of Google Test (for example, into /usr/local)?"
# at http://code.google.com/p/googletest/wiki/FAQ for more details.
add_library(gmock STATIC gmock/gmock-gtest-all.cc)
target_link_libraries(gmock "pthread")

enable_testing()
add_subdirectory(test)

# INSTALL
install(DIRECTORY ${logbox_HEADER_DIR} DESTINATION include)
install(TARGETS logbox
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
