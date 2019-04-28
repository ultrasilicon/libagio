#ifndef PCHECK_H
#define PCHECK_H

#include "parsley.h"

P_NS_BEGIN

class Check
    : public PUvObject<uv_check_t, Check>
{
  static void checkCb(uv_check_t *r);

public:
  CallbackHandler<void()> onCalled;

  Check(Loop *l);
  ~Check();
  int start();
  int stop();
};



P_NS_END
#endif // PCHECK_H
