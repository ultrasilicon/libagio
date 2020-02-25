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
  static AType* getAgioService(UvType* handle)
  {
    return static_cast<AgioServiceData<AType>*>(handle->data)->agioObj;
  }

  AgioService(Loop* loop, AType* agioObj)
    : AgioObject<UvType, AType>()
    , loop_(loop)
    , service_data_(new AgioServiceData<AType>{agioObj})
  {
    AgioObject<UvType, AType>::obj_->data = service_data_;
  }

  ~AgioService()
  {
    if(service_data_)
      delete service_data_;
  }

  template<typename T>
  T serviceData()
  {
    return this->obj_->data;
  }

  Loop* loop()
  {
    return loop_;
  }

protected:
  Loop* loop_;
  //! This class keeps a pointer to AgioObject<UvType, AType>::obj_->data.
  //! Because uv_loop might not free the data pointer when destructing objects.
  //! Above is not proved, if crash on destruct happens, try removing the delete in ~AgioService().
  AgioServiceData<AType>* service_data_;
};


A_NS_END
#endif // AGIO_SERVICE_H
