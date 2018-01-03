#pragma once

#include <ostream>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <tuple>
#include <functional>

#include "ip_v4.h"
#include "array_size.h"

class ip_pool
{
public:
  using container_type = std::vector< ip_v4 >;

  ip_pool();
  explicit ip_pool(container_type const &);
  explicit ip_pool(container_type &&);
  explicit ip_pool(std::initializer_list< ip_v4> list);
  ~ip_pool();

  auto begin() { return std::begin(addresses_); }
  auto end() { return std::end(addresses_); }

  auto size() const { return addresses_.size(); }
  void push_back(ip_v4 const &);

  ip_pool reverse_sort() const;

  template< typename ... Args >
  ip_pool filter(Args const & ...) const;

  template< typename T >
  ip_pool filter_any(T) const;

  template< typename CharT, typename Traits >
  friend decltype(auto) operator<<(std::basic_ostream< CharT, Traits > & os, ip_pool const & pool)
  {
    for (auto ip : pool.addresses_) {
      os << ip << '\n';
    }
    return os;
  }

private:
  container_type addresses_;
};

namespace detail {

template < typename ... Args >
auto make_checkers(Args ... args)
{
  return std::make_tuple(std::bind(std::equal_to<>(), args, std::placeholders::_1) ... );
}

template < typename F, size_t ... Is >
constexpr auto index_apply_impl(F f,
                                std::index_sequence< Is ... >) {
  return f(std::integral_constant< size_t, Is >{} ... );
}

template < size_t N, typename F >
constexpr auto index_apply(F f) {
  return index_apply_impl(f, std::make_index_sequence< N >{});
}

template< typename T >
auto var_and(T const & arg)
{
  return arg;
}

template< typename T, typename ... Args >
auto var_and(T const & arg, Args const & ... args)
{
  return arg && var_and(args...);
}

template< typename Checkers, typename Array, typename Operation >
constexpr auto apply_checkers(Checkers const & checkers, Array const & array, Operation op)
{
  return index_apply< std::min(std::tuple_size< Checkers >::value, array_size(array)) >(
      [&](auto ... Is) {
        return op(std::get< Is >(checkers)(array[Is])...);
    });
}

}

template < typename ... Args >
ip_pool ip_pool::filter(Args const & ... args) const
{
  static_assert(sizeof ... (Args) <= array_size(ip_v4::bytes_type()),
                "The number of args is greater then number of otcets");

  if (sizeof ... (Args) == 0) return *this;

  auto checkers = detail::make_checkers(args...);

  container_type container;

  std::copy_if(std::begin(addresses_), std::end(addresses_), std::back_inserter(container),
               [&checkers](auto const & it) {
                 return detail::apply_checkers(checkers, it.to_bytes(),
                                               [](auto const & ... args) {
                                                 return detail::var_and(args...);
                                               });
               });

  return ip_pool(container);
}
