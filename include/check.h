#ifndef PCHECK_H
#define PCHECK_H

#include "agio.h"

A_NS_BEGIN

class Check
    : public UvObject<uv_check_t, Check>
{
  static void checkCb(uv_check_t *r);

public:
  CallbackHandler<void()> onCalled;

  Check(Loop *l);
  ~Check();
  int start();
  int stop();
};



A_NS_END
#endif // PCHECK_H
