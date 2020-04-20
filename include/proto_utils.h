#ifndef AGIO_PROTOCOL_UTILS_H
#define AGIO_PROTOCOL_UTILS_H

#include "variant.h"
#include "buffer.h"

#include <cstring>
#include <string>
#include <vector>
#include <variant>
#include <unordered_map>
#include <cstdarg>
#include <cassert>

A_NS_BEGIN

namespace ProtoUtils {
  using pe_str_len_t = uint32_t;

#pragma pack(1)
  template <typename T>
  struct SizedMask {
    T header;
    char* data;
  };
#pragma pack()

  template <typename Header>
  constexpr Header scopeLen(const char* stream)
  {
    return (reinterpret_cast<const SizedMask<Header>*>(stream))->header;
  }

  template <typename Header>
  constexpr char* scopeBegin(char* stream)
  {
    return stream + sizeof(Header);
  }

  template <typename Header>
  constexpr char* scopeEnd(char* stream)
  {
    return scopeBegin<Header>(stream) + scopeLen<Header>(stream);
  }

  template <typename Header>
  constexpr std::pair<char*, char*> getScope(const char *stream)
  {
    return {scopeBegin<Header>(stream), scopeEnd<Header>(stream)};
  }

  template <typename T>
  T redeemVal(char*& stream, const char* end)
  {
    if(stream == end)
      return T();
    SizedMask<T>* p = (reinterpret_cast<SizedMask<T>*>(stream));
    stream += sizeof(T);
    return p->header;
  }

  template <typename Header>
  std::string redeemStr(char*& stream, const char* end)
  {
    if(stream == end)
      return "";
    auto ptr = stream + sizeof(Header);
    auto size = scopeLen<Header>(stream);
    stream += sizeof(Header) + scopeLen<Header>(stream);
    return std::string(ptr, size);
  }

  template <typename T>
  void appendVal(std::vector<char>& stream, size_t& pos, const T& v)
  {
    stream.resize(stream.size() + sizeof(T));
    std::memcpy(&((SizedMask<T>*) &stream[pos])->header, &v, sizeof(T));
    pos += sizeof(T);
  }

  template <>
  void appendVal<std::string>(std::vector<char>& stream, size_t& pos, const std::string& s)
  {
    stream.resize(stream.size() + sizeof(pe_str_len_t) + s.length());
    auto len = s.length();
    std::memcpy(&((SizedMask<std::string>*) &stream[pos])->header, &len, sizeof(pe_str_len_t));
    pos += sizeof(pe_str_len_t);
    std::memcpy(&stream[pos], s.c_str(), s.length());
    pos += s.length();
  }



//  using variant_t = Agio::Variant<
//  bool
//  , int8_t
//  , int16_t
//  , int32_t
//  , int64_t
//  , uint8_t
//  , uint16_t
//  , uint32_t
//  , uint64_t
//  , std::string
//  >;

  template <typename... Ts>
  struct MessageScheme {
    int msg_type_ = 0;
  };

  struct Packet
  {
    typedef Variant<
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
        > VariantT;

    std::vector<VariantT> data_;
    uint8_t msg_type_;
  };


  template <typename T>
  constexpr size_t field_size(const T&) {
    return sizeof(T);
  }

  template <>
  size_t field_size<std::string>(const std::string& str) {
    return str.size();
  }

  template <>
  size_t field_size<Buffer>(const Buffer& buf) {
    return buf.size();
  }

  template <typename T>
  constexpr size_t field_size(const T*) {
    return sizeof(T);
  }

  template <>
  size_t field_size<char>(const char* str) {
    return strlen(str);
  }

  template <typename... Ts>
  class Message
  {

  public:
    Message(Ts... fields)
      : data_(fields...)
    {
      (*this << ... << fields);
    }

    size_t size() const {return size_;}

  private:
    template <typename T>
    Message<Ts...>& operator<<(const T& t)
    {
      size_ += field_size(t);
      return *this;
    }

    std::tuple<Ts ...> data_;
    size_t size_ = 0;
  };


  class PacketComposer
  {
  public:
    PacketComposer(Buffer* buf)
      : buf_(buf)
    {}

    template<typename... Ts>
    void compose(const Message<Ts...>& message) {

    }



  private:
    Buffer* buf_;
  };



  template<typename Key>
  struct PacketMap
  {
    PacketMap(Packet* p, std::initializer_list<Key> ks)
      : packet(p)
    {
      int i = 0;
      for(const auto& k : ks)
        map.insert({k, i++});
    }

    constexpr Packet::VariantT& operator[](const Key &idx) {
      return packet->data_[map[idx]];
    }

    constexpr const Packet::VariantT& operator[](const std::size_t &idx) const {
      return packet->data_[map[idx]];
    }

    std::unordered_map<Key, int> map;
    Packet* packet;
  };


  template <typename... Ts>
  constexpr static int msgFieldCount(const MessageScheme<Ts...>&)
  {
    return std::tuple_size<std::tuple<Ts...>>::value;
  }






  template <typename Scheme, typename... Schema>
  struct Parser {
    std::unordered_map<int, std::variant<Scheme, Schema...>> schema_;

    Parser(Scheme& h, Schema&... t) {
      constructHelper(h, t...);
    }

    template<typename H>
    void constructHelper(H& h) {
      schema_.insert({h.msg_type_, h});
    }

    template<typename H, typename... T>
    void constructHelper(H& h, T&... t) {
      //      schema_.insert({h.msg_type_,  std::variant<Scheme, Schema...>{h}});
      constructHelper(t...);
    }

    Packet* decode(char* stream) {
      A_USED(stream);
      return nullptr;
    }

    template<typename H>
    H& messageSchemeHelper(const int& index, std::tuple<H>&)
    {
      //! Fail if msgType is larger than given number of schema
      assert(index == 0);
      return std::tuple<H>();
    }

    template<typename R, typename H, typename... T>
    R& messageSchemeHelper(const int& index, std::tuple<H, T...>&)
    {
      if(index == 0)
        return std::tuple<H>();
      return messageSchemeHelper(index - 1, std::tuple<T...>());
    }

    template<typename R>
    R& messageScheme(const int& msgType)
    {
      return messageSchemeHelper(msgType + 1, std::tuple<Scheme, Schema...>());
    }

    template<typename H>
    std::vector<char>& encode_helper(
          const MessageScheme<H>&,
          Packet* packet,
          std::vector<char>& stream,
          int& pos,
          const int& index)
    {
      //! could be moved to compile-time with sfinae.
      if(std::is_same<H, std::string>::value)
        appendVal(stream, pos, std::get<H>(packet->data_[index]));
      else
        appendVal(stream, pos, std::get<H>(packet->data_[index]));
      return stream;
    }

    template<typename H, typename... T>
    std::vector<char>& encode_helper(
          const MessageScheme<H, T...>&,
          Packet* packet,
          std::vector<char>& stream,
          int& pos,
          const int& index)
    {
      encode_helper(stream, pos, MessageScheme<H>{}, packet, index + 1);
      return encode_helper(stream, pos, MessageScheme<T...>{}, packet, index + 1);
    }

    //    template<typename... Ts>
    //    std::vector<char>& encode(Packet* packet) {
    //      using MessageSchemeType = decltype (messageScheme(packet->msg_type_));
    //      const MessageSchemeType& scheme = std::get<MessageSchemeType>(schema_.at(packet->msg_type_));

    //      static_assert (msgFieldCount(scheme) == packet->data.size(), "char* Parser::encode(Packet* packet): packet data field count different from declared in MessageScheme");

    //      std::vector<char> stream(sizeof(uint32_t) + 1); // vector as memory management helper
    //      size_t pos = sizeof(uint32_t);
    //      insertVal(stream, pos, packet->msg_type_);

    //      return encode_helper(scheme, pos, scheme, packet, 0);
    //    }
  };

  template <typename Scheme, typename... Schema>
  Parser<Scheme, Schema...>* make_parser(Scheme scheme, Schema... schema) {
    return new Parser<Scheme, Schema...>(scheme, schema...);
  }
}



template<typename T, typename... Ts>
struct MsgTypeTagger
    : tagger<T>
    , MsgTypeTagger<Ts...>
{
  MsgTypeTagger() : tagger<T>(sizeof...(Ts)) {}
};

template<typename T>
struct MsgTypeTagger<T>
    : tagger<T>
{
  MsgTypeTagger(const int& label) : tagger<T>(label) {}
};


template<typename... Ts>
class Parser2 {
  Parser2(const int& msgType)
    : msg_type_(msgType)
  {

  }

  template<typename H>
  Parser2<H, Ts...> reg(const H&, const int& label)
  {
    return Parser2<H, Ts...>();
  }

private:
  int msg_type_;
  tag_of_type<Ts...> tagger_;
};





A_NS_END
#endif // AGIO_PROTOCOL_UTILS_H
