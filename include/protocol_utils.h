#ifndef AGIO_PROTOCOL_UTILS_H
#define AGIO_PROTOCOL_UTILS_H

#include "variant.h"

#include <cstring>
#include <string>
#include <vector>
#include <unordered_map>
#include <cstdarg>

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

  template <typename Header>
  std::string redeemStr(char*& stream, const char* end)
  {
    if(stream == end)
      return "";
    auto ptr = stream + sizeof(Header);
    auto size = scopeLen<Header>(stream);
    std::string r(ptr, size);
    stream += sizeof(Header) + scopeLen<Header>(stream);
    return r;
  }

  template <typename T>
  void insertVal(std::vector<char>& stream, size_t& pos, const T& v)
  {
    stream.resize(stream.size() + sizeof(T));
    std::memcpy(&((SizedMask<T>*) &stream[pos])->header, &v, sizeof(T));
    pos += sizeof(T);
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



using variant_t = Agio::Variant<
    bool
    , int8_t
    , int16_t
    , int32_t
    , int64_t
    , uint8_t
    , uint16_t
    , uint32_t
    , uint64_t
    , std::string
>;


struct Packet
{
  std::vector<variant_t> data;
  uint8_t msg_type_;
};

template <typename... Seq>
struct MessageScheme {
  int msg_type_ = 0;
};

template <typename Scheme, typename... Schema>
struct Parser {
  std::unordered_map<int, Variant<Schema...>> schema_;

  Parser(Scheme& scheme) {
    schema_.insert({scheme.message_type_, scheme});
  }

  Parser(Scheme& scheme, Schema&... schema) {
    schema_.insert({scheme.message_type_, scheme});
    Parser(schema...);
  }

  Packet* decode(char* stream) {
    A_USED(stream);
    return nullptr;
  }


  template<typename T>
  char* encode_helper(char* pos, T& t) {
    A_USED(pos);
    A_USED(t);
    return pos;
  }

  template<typename T, typename... Ts>
  char* encode_helper(char* pos, T& t, Ts&... ts) {
    A_USED(pos);
    A_USED(t);
    return pos;
  }

  char* encode(Packet* packet) {
    auto scheme = schema_.at(packet->msg_type_);


    return encode_helper(scheme);
  }
};

template <typename Scheme, typename... Schema>
Parser<Scheme, Schema...>* make_parser(Scheme scheme, Schema... schema) {
  return new Parser<Scheme, Schema...>(scheme, schema...);
}

template<typename T>
void expected()
{
  MessageScheme<
      int32_t
      > schemeHeartbeat{0};

  MessageScheme<
      std::string
      > schemeAuthReq{1};

  MessageScheme<
      std::string
      > schemeAuthRes{2};

  MessageScheme<
      std::string
      > schemeLoginReq{3};

  MessageScheme<
      bool,
      std::string
      > schemeLoginRes{4};

  MessageScheme<
      std::string,
      int32_t,
      std::string
      > schemeMsgTxt{5};

  auto* parser = make_parser(schemeHeartbeat,
                        schemeAuthReq,
                        schemeAuthRes,
                        schemeLoginReq,
                        schemeLoginRes,
                        schemeMsgTxt);
}



A_NS_END
#endif // AGIO_PROTOCOL_UTILS_H
