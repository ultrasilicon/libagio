#include "address.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <climits>

using namespace std;
using namespace Agio;

namespace HostAddressTestHelper
{
}

TEST(StreamOperator, OStream)
{
  cout << HostAddress("127.0.0.1", 0) << endl;
  EXPECT_EQ("", string());
}

TEST(BufferCopy, FromPosToPos)
{
  EXPECT_EQ("", string());
}

TEST(BufferCopy, FromPosToLen)
{
  EXPECT_EQ("", string());
}




