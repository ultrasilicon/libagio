#ifndef AGIO_SERVICE_H
#define AGIO_SERVICE_H

#include "loop.h"

A_NS_BEGIN

template <typename AType>
struct AgioServiceData
{
  AType* agioObj;
  void* data;
  // ...
};

template <typename UvType, typename AType>
class AgioService
    : public AgioObject<UvType, AType>
{
public:
  static AType* getPHandle(UvType* handle)
  {
    return static_cast<AgioServiceData<AType>*>(handle->data)->agioObj;
  }

  AgioService(Loop* loop, AType* agioObj)
    : AgioObject<UvType, AType>()
    , loop_(loop)
  {
    AgioObject<UvType, AType>::obj_->data = new AgioServiceData<AType>{agioObj};
  }

  ~AgioService()
  {
    delete AgioObject<UvType, AType>::obj_->data;
  }

  template<typename T>
  T getServiceData()
  {
    return this->obj_->data;
  }

  Loop* getLoop()
  {
    return loop_;
  }

protected:
  Loop* loop_;
};


A_NS_END
#endif // AGIO_SERVICE_H
