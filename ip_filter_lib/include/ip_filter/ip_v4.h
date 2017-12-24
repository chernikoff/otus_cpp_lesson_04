#pragma once

#include <array>
#include <string>
#include <cstdint>
#include <ostream>

class ip_v4
{
public:
  using byte_type = uint8_t;
  using bytes_type = std::array< byte_type, 4 >;

  ip_v4();
  ip_v4(bytes_type const &);
  ip_v4(bytes_type &&);
  ~ip_v4();

  static ip_v4 make(std::string const & string);

  bytes_type const & to_bytes() const;
  std::string to_string() const;

  template< typename CharT, typename Traits >
  friend decltype(auto) operator<<(std::basic_ostream< CharT, Traits > & os, ip_v4 const & ip)
  {
    os << ip.to_string();
    return os;
  }

private:
  bytes_type bytes_;
};
