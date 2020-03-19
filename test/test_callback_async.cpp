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
  cb = [&](int a){
      if(a == 3) {
          cout << a << " recursive rounds passed" << endl;
          cb.close();
          return;
        }
      cb(a + 1);
    };

  cb(0)/*.then([](){
      printf("then 1\n");
    }).then([](){
      printf("then 2\n");
    }).finally([](){
      printf("finally 3\n");
    }).err([](){
      printf("error\n");
    })*/;

  loop->run();
  EXPECT_EQ("", string());
}






