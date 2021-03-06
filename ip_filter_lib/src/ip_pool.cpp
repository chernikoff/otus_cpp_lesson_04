#include "ip_pool.h"

ip_pool::ip_pool()
{}

ip_pool::ip_pool(ip_pool && other)
{
  std::swap(addresses_, other.addresses_);
}

ip_pool::ip_pool(ip_pool const & other)
    : addresses_(other.addresses_)
{}

ip_pool::ip_pool(container_type const & c)
  : addresses_(c)
{}

ip_pool::ip_pool(container_type && c)
  : addresses_(c)
{}

ip_pool::ip_pool(std::initializer_list< ip_v4 > list)
  : addresses_(list)
{}

ip_pool::~ip_pool()
{}

ip_pool & ip_pool::operator=(ip_pool && other)
{
  std::swap(addresses_, other.addresses_);
  return *this;
}

ip_pool & ip_pool::operator=(ip_pool const & other)
{
  auto copy(other);
  *this = std::move(copy);
  return *this;
}

void ip_pool::push_back(ip_v4 const & ip)
{
  addresses_.push_back(ip);
}

ip_pool ip_pool::reverse_sort() const
{
  auto addresses_copy = addresses_;

  std::sort(std::begin(addresses_copy), std::end(addresses_copy),
            [](auto const & lhs, auto const & rhs) {
              return lhs.to_bytes() > rhs.to_bytes();
            });

  return ip_pool(std::move(addresses_copy));
}
