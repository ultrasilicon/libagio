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
  CallbackAsync<void(int)> cb([](int a){cout << "hello" << a << endl;}, loop);

  Timer* timer = new Timer(1,300, loop);
  on(&timer->onTimedOut, [&](Timer*){
      static int i = 0;
      cb(i);
    });

  timer->start();
  loop->run();
  EXPECT_EQ("", string());
}






