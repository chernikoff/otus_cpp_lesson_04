#include <gtest/gtest.h>

#include <stdexcept>
#include <ip_filter/ip_v4.h>

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

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


