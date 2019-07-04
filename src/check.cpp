#include "check.h"

using namespace Parsley;


void Check::checkCb(uv_check_t *r)
{
  getInstance(r)->onCalled();
}


Check::Check(Loop *l)
  : PUvObject(l, this)
{
  uv_check_init(l->uvHandle(), obj_);
  regInstance(obj_, this);
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
