#include "PCheck.h"

using namespace Parsley;

CheckUtils::CheckUtils(Loop *l)
  : PObject(l)
{
}

void CheckUtils::checkCb(uv_check_t *r)
{
  getInstance(r)->onCalled.call();
}


Check::Check(Loop *l)
  : CheckUtils(l)
{
  uv_check_init(l->uvHandle(), m_uv_handle);
  regInstance(m_uv_handle, this);
}

Check::~Check()
{
}

int Check::start()
{
  return uv_check_start(m_uv_handle
                        , checkCb);
}

int Check::stop()
{
  return uv_check_stop(m_uv_handle);
}
