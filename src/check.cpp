#include "check.h"

using namespace Agio;


void Check::checkCb(uv_check_t *r)
{
  getPHandle(r)->onCalled();
}


Check::Check(Loop *l)
  : AgioService(l, this)
{
  uv_check_init(l->cObject(), obj_);
}

Check::~Check()
{
}

int Check::start()
{
  return uv_check_start(obj_, checkCb);
}

int Check::stop()
{
  return uv_check_stop(obj_);
}
