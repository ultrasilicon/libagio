#ifndef PCHECK_H
#define PCHECK_H

#include "Parsley.h"
#include "PLoop.h"

PARSLEY_NAMESPACE_BEGIN
class CheckUtils;
class Check;

class TimerUtils
    : public PObject<uv_check_t, Check>
{

};

class Check
    : TimerUtils
{
public:
  Check(Loop *l);
};



PARSLEY_NAMESPACE_END
#endif // PCHECK_H
