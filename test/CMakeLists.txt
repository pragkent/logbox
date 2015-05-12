set(TEST_MAIN_SRC test_main.cc)
add_library(test-main STATIC ${TEST_MAIN_SRC})
target_link_libraries(test-main logbox gmock)

set(LOGBOX_UTIL_TEST_SRC
  timestamp_test.cc
  slice_test.cc
  posix_error_test.cc
  environment_test.cc)

add_executable(logbox-util-test ${LOGBOX_UTIL_TEST_SRC})
target_link_libraries(logbox-util-test test-main logbox)
add_test(logbox-util-test logbox-util-test)
set_target_properties(logbox-util-test PROPERTIES COMPILE_FLAGS
  "-std=c++11 -Wall -Wextra -pedantic -Wno-long-long -Wno-variadic-macros")
