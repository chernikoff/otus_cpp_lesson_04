#include <gtest/gtest.h>

#include <vector>
#include <string>
#include <stdexcept>
#include <ip_filter/ip_v4.h>
#include <ip_filter/ip_pool.h>

TEST(ip_v4, constuction)
{
  EXPECT_EQ(ip_v4(ip_v4::bytes_type{192, 168, 1, 10}).to_string(),
            "192.168.1.10");

  ip_v4::bytes_type bytes = {192, 168, 5, 100};
  EXPECT_EQ(ip_v4::make("192.168.5.100").to_bytes(),
            bytes);

  EXPECT_THROW(ip_v4::make(""), std::logic_error);
  EXPECT_THROW(ip_v4::make("192.168.0.l"), std::logic_error);
  EXPECT_THROW(ip_v4::make("it_s_word"), std::logic_error);
  EXPECT_THROW(ip_v4::make("192.16.8.4.43"), std::logic_error);
}

TEST(ip_pool, reverse_sort)
{
  ip_pool pool {
    ip_v4::make("1.1.1.1"),
    ip_v4::make("192.168.1.1"),
    ip_v4::make("192.168.1.2"),
    ip_v4::make("200.1.5.0")
  };

  pool = pool.reverse_sort();

  std::vector< std::string > template_{
    "200.1.5.0",
    "192.168.1.2",
    "192.168.1.1",
    "1.1.1.1"
  };

  EXPECT_EQ(pool.size(), template_.size());

  for (auto it = std::make_pair(std::begin(pool), std::begin(template_)),
         end = std::make_pair(std::end(pool), std::end(template_));
       it.first != end.first; ++it.first, ++it.second) {
    EXPECT_EQ(it.first->to_string(), *it.second);
  }
}

TEST(ip_pool, filter)
{
  ip_pool pool {
    ip_v4::make("1.1.1.1"),
    ip_v4::make("192.168.1.1"),
    ip_v4::make("192.172.4.44"),
    ip_v4::make("192.168.1.2"),
    ip_v4::make("200.1.5.0")
  };

  auto filtred = pool.filter();
  std::vector< std:: string > template_ = {
    "1.1.1.1",
    "192.168.1.1",
    "192.172.4.44",
    "192.168.1.2",
    "200.1.5.0"
  };

  auto test_routine = [&]() {
    EXPECT_EQ(filtred.size(), template_.size());

    for (auto it = std::make_pair(std::begin(filtred), std::begin(template_)),
             end = std::make_pair(std::end(filtred), std::end(template_));
         it.first != end.first; ++it.first, ++it.second) {
      EXPECT_EQ(it.first->to_string(), *it.second);
    }
  };

  test_routine();

  filtred = pool.filter(200);

  template_ = {
    "200.1.5.0"
  };

  test_routine();

  filtred = pool.filter(192, 168);
  template_ = {
    "192.168.1.1",
    "192.168.1.2"
  };

  test_routine();
}

TEST(ip_pool, filter_any)
{
  ip_pool pool {
    ip_v4::make("1.1.1.1"),
        ip_v4::make("192.168.1.1"),
        ip_v4::make("192.172.4.44"),
        ip_v4::make("192.168.1.2"),
        ip_v4::make("200.1.5.168")
        };

  auto filtred = pool.filter_any(168);
  std::vector< std:: string > template_ = {
    "192.168.1.1",
    "192.168.1.2",
    "200.1.5.168"
  };

  auto test_routine = [&]() {
    EXPECT_EQ(filtred.size(), template_.size());

    for (auto it = std::make_pair(std::begin(filtred), std::begin(template_)),
             end = std::make_pair(std::end(filtred), std::end(template_));
         it.first != end.first; ++it.first, ++it.second) {
      EXPECT_EQ(it.first->to_string(), *it.second);
    }
  };

  test_routine();
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


