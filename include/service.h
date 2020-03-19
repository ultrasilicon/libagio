#ifndef AGIO_SERVICE_H
#define AGIO_SERVICE_H

#include "loop.h"

A_NS_BEGIN

template <typename AgioT>
struct ServiceData
{
  AgioT* agio_obj_;
  void* data_;
  // ...
};

template <typename UvT, typename AgioT, typename ServiceDataT = ServiceData<AgioT>>
class AgioService
    : public AgioObject<UvT, AgioT>
{
public:
  using AgioObjectT = AgioObject<UvT, AgioT>;

  static AgioT* getAgioService(UvT* handle)
  {
    return static_cast<ServiceDataT*>(handle->data)->agio_obj_;
  }

  AgioService(Loop* loop, AgioT* agioObj)
    : AgioObjectT()
    , service_data_(new ServiceDataT{agioObj})
    , loop_(loop)
  {
    this->obj_->data = service_data_;
  }

//  ~AgioService()
//  {
//    if(service_data_)
//        delete service_data_;
//    service_data_ = nullptr;
//  }

  ServiceDataT* serviceData()
  {
    return static_cast<ServiceDataT*>(AgioObjectT::obj_->data);
  }

  Loop* loop()
  {
    return loop_;
  }

  //! TODO: Check for memory leak
  void close() {
    uv_close(reinterpret_cast<uv_handle_t*>(AgioObjectT::obj_), nullptr);
  }

protected:
  //! This class keeps a pointer to AgioObject<UvType, AType>::obj_->data.
  //! Because uv_loop might not free the data pointer when destructing objects.
  //! Above is not proved, if crash on destruct happens, try removing the delete in ~AgioService().
  ServiceDataT* service_data_;
  Loop* loop_;
};


A_NS_END
#endif // AGIO_SERVICE_H
