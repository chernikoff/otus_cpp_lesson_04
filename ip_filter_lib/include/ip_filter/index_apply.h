#pragma once

template < typename F, size_t ... Is >
constexpr auto index_apply_impl(F f,
                                std::index_sequence< Is ... >) {
  return f(std::integral_constant< size_t, Is >{} ... );
}

template < size_t N, typename F >
constexpr auto index_apply(F f) {
  return index_apply_impl(f, std::make_index_sequence< N >{});
}

