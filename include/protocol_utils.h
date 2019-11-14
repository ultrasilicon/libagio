#ifndef AGIO_PROTOCOL_UTILS_H
#define AGIO_PROTOCOL_UTILS_H

#include "variant.h"

#include <cstring>
#include <string>
#include <vector>

A_NS_BEGIN

namespace ProtocolUtils {
  using pe_str_len_t = uint32_t;

#pragma pack(1)
  template <typename T>
  struct SizedMask {
    T header;
    char* data;
  };
#pragma pack()

  template <typename Header>
  Header scopeLen(const char* stream)
  {
    return (reinterpret_cast<SizedMask<Header>*>(const_cast<char*>(stream)))->header;
  }

  template <typename Header>
  char* scopeBegin(char* stream)
  {
    return stream + sizeof(Header);
  }

  template <typename Header>
  const char* scopeEnd(char* stream)
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

  //template <typename _HeaderT, typename _ValT>
  //_ValT redeemStr(char* &stream, const char* end)
  //{
  //  if(stream == end)
  //    return "";
  //  _ValT r = (_ValT)constructStr(stream + sizeof(_HeaderT),
  //                                 scopeLen<_HeaderT>(stream));
  //  stream += sizeof(_HeaderT) + scopeLen<_HeaderT>(stream);
  //  return r;
  //}

  template <typename _HeaderT>
  std::string redeemStr(char*& stream, const char* end)
  {
    if(stream == end)
      return "";
    auto ptr = stream + sizeof(_HeaderT);
    auto size = scopeLen<_HeaderT>(stream);
    std::string r(ptr, size);
    stream += sizeof(_HeaderT) + scopeLen<_HeaderT>(stream);
    return r;
  }

  template <typename _ValT>
  void insertVal(std::vector<char>& stream, size_t& pos, const _ValT& v)
  {
    stream.resize(stream.size() + sizeof(_ValT));
    std::memcpy(&((SizedMask<_ValT>*) &stream[pos])->header, &v, sizeof(_ValT));
    pos += sizeof(_ValT);
  }

  /*!
   * \brief: If this funciton is not a template, it will have compile errors, because it's not in any class.
   * Put it here for now as in the future it all these parsing functions will be moved to a pre-compile module.
   * \arg _std_string must be std::string.
   */
  template <typename _std_string>
  void insertStr(std::vector<char>& stream, size_t& pos, const _std_string& s)
  {
    stream.resize(stream.size() + sizeof(pe_str_len_t) + s.length());
    auto len = s.length();
    std::memcpy(&((SizedMask<std::string>*) &stream[pos])->header, &len, sizeof(pe_str_len_t));
    pos += sizeof(pe_str_len_t);
    std::memcpy(&stream[pos], s.c_str(), s.length());
    pos += s.length();
  }

}





A_NS_END
#endif // AGIO_PROTOCOL_UTILS_H
