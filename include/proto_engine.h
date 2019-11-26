#ifndef AGIO_PROTOCOL_PARSER_H
#define AGIO_PROTOCOL_PARSER_H

#include "agio.h"
#include "buffer.h"
#include "proto_utils.h"
#include "tcp_socket.h"

A_NS_BEGIN

using namespace ProtoUtils;

class ProtoEngine
{
public:
  CallbackHandler<void(Packet* packet)> onMessage;

  ProtoEngine(TcpSocket* socket);
  void message(Packet* packet);

private:
  std::string buffer_;
  TcpSocket* socket_;

  void read(Buffer* stream, TcpSocket* socket);
  void write(Packet* packet);
  Buffer* encode(Packet* packet);
  Packet* decode(Buffer* );
};

A_NS_END
#endif // AGIO_PROTOCOL_PARSER_H
