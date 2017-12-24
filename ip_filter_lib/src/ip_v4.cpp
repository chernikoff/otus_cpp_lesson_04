#include "ip_v4.h"

#include <stdexcept>

#include "string_utils.h"
#include "array_size.h"

ip_v4::ip_v4()
  : bytes_{0, 0, 0, 0}
{}

ip_v4::ip_v4(bytes_type const & bytes)
  : bytes_(bytes)
{}

ip_v4::ip_v4( bytes_type && bytes)
  : bytes_(bytes)
{}

ip_v4::~ip_v4()
{}

ip_v4 ip_v4::make(std::string const & string)
{
  static auto const invalid_ip_msg = "Invalid ip string";
  ip_v4::bytes_type bytes;
  auto ip_parted = split(string, '.');

  if (ip_parted.size() != array_size< decltype(bytes) >::size) {
    throw std::logic_error(invalid_ip_msg);
  }

  for (size_t i = 0, size = array_size< decltype(bytes) >::size;
       i != size; ++i) {
    auto value = std::stoi(ip_parted[i]);
    if (value < 0 || value > 255) { throw std::logic_error(invalid_ip_msg); }
    bytes[i] = static_cast< byte_type >(value);
  }

  return ip_v4(std::move(bytes));
}

ip_v4::bytes_type const & ip_v4::to_bytes() const
{
  return bytes_;
}

std::string ip_v4::to_string() const
{
  std::string string;

  for (auto begin = std::begin(bytes_), end = std::end(bytes_), it = begin;
       it != end; ++it) {
    if (it != begin) {
      string.push_back('.');
    }
    string.append(std::to_string(*it));
  }

  return string;
}
