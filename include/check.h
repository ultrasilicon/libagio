#ifndef PCHECK_H
#define PCHECK_H

#include "parsley.h"

P_NS_BEGIN
class CheckUtils;
class Check;

class CheckUtils
    : public PUvObject<uv_check_t, Check>
{
public:
  CheckUtils(Loop *l);

protected:
  static void checkCb(uv_check_t *r);

};

class Check
    : CheckUtils
{
public:
  CallbackHandler<void()> onCalled;

  Check(Loop *l);
  ~Check();
  int start();
  int stop();
};



P_NS_END
#endif // PCHECK_H
