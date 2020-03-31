#include "buffer.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <climits>

using namespace std;
using namespace Agio;

namespace BufferTestHelper
{
}

//TEST(BufferCopy, FromBeginToLen)
//{
//  EXPECT_EQ("", string());
//}

//TEST(BufferCopy, FromPosToPos)
//{
//  EXPECT_EQ("", string());
//}

TEST(BufferCopy, OStreamRef)
{
  stringstream ss;
  Buffer buf("\0B\nU\tG\\");
  ss << buf;
  EXPECT_EQ(buf.toString(), ss.str());
}

TEST(BufferCopy, OStreamPtr)
{
  stringstream ss;
  Buffer buf("\0B\nU\tG\\");
  ss << &buf;
  EXPECT_EQ(buf.toString(), ss.str());
}




