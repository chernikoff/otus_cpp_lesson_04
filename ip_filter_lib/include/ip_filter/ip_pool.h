#pragma once

#include <ostream>
#include <vector>
#include <algorithm>
#include <initializer_list>
#include <tuple>
#include <functional>
#include <type_traits>

#include "ip_v4.h"
#include "array_size.h"
#include "index_apply.h"

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

  ip_pool filter() const { return *this; }

  template< typename T >
  ip_pool filter_any(T const &) const;

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

template< typename T >
constexpr auto var_and(T const & arg)
{
  return arg;
}

template< typename T, typename ... Args >
constexpr auto var_and(T const & arg, Args const & ... args)
{
  return arg && var_and(args...);
}

template< typename T >
constexpr auto var_or(T const & arg)
{
  return arg;
}

template< typename T, typename ... Args >
constexpr auto var_or(T const & arg, Args const & ... args)
{
  return arg || var_or(args...);
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

  auto tuple = std::make_tuple(args...);
  auto checkers = index_apply< std::tuple_size< decltype(tuple) >::value >(
      [&tuple](auto ... Is) {
        return std::make_tuple(std::bind(std::equal_to<>(), std::get< Is >(tuple), std::placeholders::_1) ... );
      });

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

template< typename T >
ip_pool ip_pool::filter_any(T const & value) const
{
  auto checkers = index_apply<array_size(ip_v4::bytes_type())>(
      [&value](auto ... Is) {
        return std::make_tuple(((void)Is, std::bind(std::equal_to<>(), value, std::placeholders::_1)) ... );
      });

  container_type container;

  std::copy_if(std::begin(addresses_), std::end(addresses_), std::back_inserter(container),
               [&checkers](auto const & it) {
                 return detail::apply_checkers(checkers, it.to_bytes(),
                                               [](auto const & ... args) {
                                                 return detail::var_or(args...);
                                               });
               });

  return ip_pool(container);
}
