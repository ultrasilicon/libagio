#include "proto_engine.h"

using namespace Agio;

ProtoEngine::ProtoEngine(TcpSocket* socket)
  : socket_(socket)
{
}

void ProtoEngine::message(Packet* packet)
{

}

Buffer* ProtoEngine::encode(Packet* packet)
{

  return nullptr;
}

Packet* ProtoEngine::decode(Buffer*)
{
  return nullptr;
}
