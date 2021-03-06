#ifndef AGIO_PROTOCOL_PARSER_H
#define AGIO_PROTOCOL_PARSER_H

#define A_PROTO_ENGINE_HEADER_TYPE uint32_t
#define A_PROTO_ENGINE_HEADER_SIZE sizeof(A_PROTO_ENGINE_HEADER_TYPE)

#include "agio.h"
#include "buffer.h"
#include "proto_utils.h"
#include "tcp_socket.h"
#include "async_event.h"

#include <queue>

A_NS_BEGIN

using namespace ProtoUtils;


class ProtoEngine;
class StreamProtoEngine;


class ProtoEngine
{
public:
  using MsgSizeT = A_PROTO_ENGINE_HEADER_TYPE;
  Callback<void(Packet* packet)> onMessage;

  ProtoEngine(TcpSocket* sock, Loop* l);
  virtual ~ProtoEngine();

  virtual void message(Packet* packet) = 0;

protected:
  AsyncEvent* try_parse_;
  TcpSocket* socket_;
  std::queue<Buffer*> buffers_;

  virtual void encode(AsyncEvent* ev) = 0;
  virtual void decode(AsyncEvent* ev) = 0;
};



class StreamProtoEngine
    : public ProtoEngine
{  
  enum ParsingState : unsigned char {
    Start = 0,
    ReadingHeader,
    ReadingBody,
  };

public:
  StreamProtoEngine(TcpSocket* sock, Loop* l);
  ~StreamProtoEngine();

  void message(Packet* pkt);

private:
  char* rptr_ = nullptr;   //! pointer to a buffer in ProtoEngine::buffers_ for reading
  char* wptr_ = nullptr;   //! pointer to ParsingStates::wbuf for writing
  Buffer* wbuf_;           //! parsing buffer with length equal to the message length
  MsgSizeT msg_len_ = 0;  //! message length
  MsgSizeT read_len_ = 0; //! read message length
  ParsingState state_ = Start;  //! state name identifier

  void initStates(const MsgSizeT& len);
  void readBuf(const char* src, const MsgSizeT& size);
  MsgSizeT totalLength() const;
  const ParsingState& getId() const;

  void read(Buffer* stream, TcpSocket* sock);
  void write(Packet* pkt);
  void encode(AsyncEvent* ev);
  void decode(AsyncEvent* ev);
};

A_NS_END

#endif // AGIO_PROTOCOL_PARSER_H
