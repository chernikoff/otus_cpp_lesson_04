#pragma once

#include <array>
#include <cstdint>

template < typename T, size_t N >
size_t constexpr array_size(std::array<T, N> const &)
{
  return N;
}
