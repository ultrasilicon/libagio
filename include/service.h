#ifndef AGIO_SERVICE_H
#define AGIO_SERVICE_H

#include "loop.h"

A_NS_BEGIN

template <typename PType>
struct AgioServiceData
{
  PType* pHandle;
  // ...
};

template <typename UvType, typename PType>
class AgioService
    : public AgioObject<UvType, PType>
{
public:
  static PType* getPHandle(UvType* handle)
  {
    return static_cast<AgioServiceData<PType>*>(handle->data)->pHandle;
  }

  AgioService(Loop* l, PType* pHandle)
    : AgioObject<UvType, PType>()
    , loop_(l)
    , data_(new AgioServiceData<PType>{pHandle})
  {
    AgioObject<UvType, PType>::obj_->data = data_;
  }

  ~AgioService()
  {
    delete data_;
  }

  Loop* getLoop()
  {
    return loop_;
  }

protected:
  Loop* loop_;
  AgioServiceData<PType>* data_;
};


A_NS_END
#endif // AGIO_SERVICE_H
