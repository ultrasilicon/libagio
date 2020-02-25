#include "address.h"

#include <gtest/gtest.h>
#include <string>
#include <sstream>
#include <climits>

using namespace std;
using namespace Agio;

namespace HostAddressTestHelper
{
}


TEST(HostAddress, OStreamPortOverflow)
{
  for(int i = 0; i < 32768; ++ i) {
      stringstream ss;
      ss << HostAddress("0.0.0.0", i);
      EXPECT_EQ("0.0.0.0:" + to_string(i % UINT16_MAX), ss.str());
    }
}

TEST(StreamOperator, OStream)
{
  for(int i = 0; i < 32768; ++ i) {
      stringstream ss;
      HostAddress addr("127.0.0.1", i);
      ss << addr;
      EXPECT_EQ(addr.toString(), ss.str());
    }
}



