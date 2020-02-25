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


TEST(HostAddress, PortOverflow)
{
  for(u_long i = 0; i < 300000; ++ i) {
      HostAddress addr("0.0.0.0", i);
      EXPECT_EQ(addr.port(), i % (UINT16_MAX + 1));
    }
}

TEST(StreamOperator, OStream)
{
  for(int i = 0; i < 300000; ++ i) {
      stringstream ss;
      HostAddress addr("127.0.0.1", i);
      ss << addr;
      EXPECT_EQ(addr.toString(), ss.str());
    }
}



