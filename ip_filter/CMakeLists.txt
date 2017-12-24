add_executable(ip_filter
  src/ip_filter.cpp
  )

set_target_properties(ip_filter
  PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS NO
  )

target_compile_options(ip_filter
  PUBLIC
  -Werror -Wpedantic -Wall -Wextra)

target_link_libraries(ip_filter ip_filter_lib)

install(TARGETS ip_filter
  RUNTIME DESTINATION bin)

