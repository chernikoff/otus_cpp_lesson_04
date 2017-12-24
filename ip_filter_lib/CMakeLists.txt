add_library(ip_filter_lib
  )

set_target_properties(ip_filter_lib
  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS NO
  )

set_target_properties(ip_filter_lib
  PROPERTIES
  OUTPUT_NAME "ip_filter"
  )

target_compile_options(ip_filter_lib
  PUBLIC
  -Werror -Wpedantic -Wall -Wextra)

target_include_directories(ip_filter_lib
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include/ip_filter
  )

target_include_directories(ip_filter_lib
  PUBLIC
  include)

if(GTEST_FOUND)
  add_subdirectory(test)
endif()
