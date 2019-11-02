#ifndef AGIO_VARIANT_H
#define AGIO_VARIANT_H

#include "agio.h"

A_NS_BEGIN

template<size_t _MAX, typename T, typename... Ts>
struct max_type_size : max_type_size<(_MAX <= sizeof(T) ? sizeof(T) : _MAX), Ts...>
{
};
template<size_t _MAX, typename T>
struct max_type_size<_MAX, T>
{
    static constexpr size_t value = _MAX <= sizeof(T) ? sizeof(T) : _MAX;
};

template<typename T>
struct tagger
{
  int value;
  tagger(size_t s) : value(s) {}
};

template<typename T, typename... Ts>
struct tag_of_type
    : tagger<T>
    , tag_of_type<Ts...>
{
    tag_of_type() : tagger<T>(sizeof...(Ts)) {}
};

template<typename T>
struct tag_of_type<T>
    : tagger<T>
{
    tag_of_type() : tagger<T>(0) {}
};

template<typename... Ts>
struct Variant
{
  const size_t size_ = max_type_size<0,Ts...>::value;
  tag_of_type<Ts...> tagger_;
  int curr_tag_ = -1;
  char* data_;

  template<typename T>
  Variant(const T& v)
    : data_(new char[size_]{})
  {
    new (data_) T(v);
    curr_tag_ = static_cast<tagger<T>&>(tagger_).value;
  }

  Variant()
    : Variant(0)
  {}

  Variant(const Variant& that)
    : data_(new char[size_]{})
  {
    for(int i = 0; i < size_; i++)
        data_[i] = that.data_[i];
    curr_tag_ = that.curr_tag_;
  }

  ~Variant()
  {
    if(data_)
      delete[] data_;
    data_ = nullptr;
  }

  template<typename T>
  Variant& operator=(const T& v) = delete;
//    {
////        if(!std::is_same<T, std::string>::value
////                && std::is_base_of<tagger<std::string>, variant>::value
////                && m_curr_tag == static_cast<tagger<std::string>&>(m_tagger).value) // previous and now both strings?
////            ((std::string*)m_p)->~std::basic_string<char>();
//        *(T*)m_p = v;
//        m_curr_tag = static_cast<tagger<T>&>(m_tagger).value;
//        return *this;
//    }

  template<typename T>
  T& get()
  {
      if(curr_tag_ != static_cast<tagger<T>&>(tagger_).value)
          throw std::bad_cast{};
      return *(T*)data_;
  }
};


A_NS_END
#endif // AGIO_VARIANT_H
