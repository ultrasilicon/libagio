#ifndef AGIO_PROTOCOL_UTILS_H
#define AGIO_PROTOCOL_UTILS_H

#include "variant.h"

A_NS_BEGIN

namespace ProtocolUtils {
#pragma pack(1)
  template <typename T>
  struct SizedMask {
    T header;
    char* data;
  };
#pragma pack()

  template <typename Header>
  Header scopeLen(const char *stream)
  {
    return (reinterpret_cast<SizedMask<Header>*>(const_cast<char*>(stream)))->header;
  }

  template <typename Header>
  char* scopeBegin(char *stream)
  {
    return stream + sizeof(Header);
  }

  template <typename Header>
  const char* scopeEnd(char *stream)
  {
    return scopeBegin<Header>(stream) + scopeLen<Header>(stream);
  }

  template <typename Header>
  std::pair<char*, char*> getScope(char *stream)
  {
    return std::make_pair(scopeBegin<Header>(stream), scopeEnd<Header>(stream));
  }

  template <typename T>
  T redeemVal(char*& stream, const char* end)
  {
    if(stream == end)
      return T();
    SizedMask<T>* p = (reinterpret_cast<SizedMask<T>*>(stream));
    T r = p->header;
    stream += sizeof(T);
    return r;
  }
}


A_NS_END
#endif // AGIO_PROTOCOL_UTILS_H
