#pragma once

#include <array>
#include <cstdint>

template< typename >
struct array_size;

template< typename T, size_t N >
struct array_size< std::array< T, N > >
{
  static size_t const size = N;
};
