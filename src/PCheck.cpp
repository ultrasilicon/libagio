#include "PCheck.h"

using namespace Parsley;

void CheckUtils::checkCb(uv_check_t *r)
{
  getInstance(r)->callCheck();
}


Check::Check(Loop *l)
  : CheckUtils(l)
{
  uv_check_init(l->uvHandle(), uv_handle);
  addInstance(uv_handle, this);
}

Check::~Check()
{

}

int Check::start()
{
  return uv_check_start(uv_handle
                        , checkCb);
}

int Check::stop()
{
  return uv_check_stop(uv_handle);
}

void Check::bindCb(const CheckUtils::CheckCb &cb)
{
  check_cb = cb;
}

bool Check::callCheck()
{
  if(check_cb)
    {
      check_cb();
      return true;
    }
  return false;
}


