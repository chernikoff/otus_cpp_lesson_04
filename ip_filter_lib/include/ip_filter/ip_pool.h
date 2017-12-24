#pragma once

#include <ostream>
#include <vector>
#include <algorithm>
#include <initializer_list>

#include "ip_v4.h"

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

  void push_back(ip_v4 const &);

  ip_pool reverse_sort() const;

  template< typename ... Args >
  ip_pool filter(Args ...) const;

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

