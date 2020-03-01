#include "callback.h"
#include "callback_async.h"
#include "timer.h"

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <climits>

using namespace std;
using namespace Agio;

namespace BufferTestHelper
{
}

TEST(CallbackAsync, Constructor)
{
  Loop* loop = Loop::defaultLoop();
  CallbackAsync<void(int)> cb(loop);
  cb.connect([&](int a){
      if(a == 1000000) {
          cb.close();
          cout << a << '\n';
          return;
        }
      cb(a + 1);
    });
  cb(0);

  loop->run();
  EXPECT_EQ("", string());
}






