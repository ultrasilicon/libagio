#include "PCheck.h"

using namespace Parsley;

Check::Check(Loop *l)
{
  uv_handle = (uv_check_t*) malloc(sizeof(uv_check_t));
  uv_check_init(l->uvHandle(), uv_handle);

  regInstance(uv_handle, this);
}
